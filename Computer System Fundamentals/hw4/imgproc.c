#include <stdio.h>
#include "image.h"
#include <dlfcn.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

//Plugin Struct
struct Plugin {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
};

char *getPluginDirectory() {
    char *pluginDir;
    if(getenv("PLUGIN_DIR")) { //Check for environment variable for plugin path
        pluginDir = getenv("PLUGIN_DIR"); //Save directory for later
    } else { //If it doesnt exist use default
        pluginDir = "./plugins"; //Save directory for later
    }
    return pluginDir;
}


//Returns an array of all available plugins and puts the amount of plugins in the pointer provided
struct Plugin * getPluginList(int *pluginCount) {
    DIR * directory;
    char *pluginDir = getPluginDirectory();
    directory = opendir(pluginDir);
    struct dirent * entry; //Create a dirent struct called entry to handle each file in directory
    struct Plugin *dest = malloc(sizeof(struct Plugin)); //The destination array we will return
    while((entry = readdir(directory)) != NULL) { //While there is another file in the directory
        if(entry->d_type == 8) { //Check if its a .so
            char currentFile[1024]; //char to add path too
            strcpy(currentFile, "");//concatenate emptiness first
            strcat(currentFile, pluginDir); //Add the directory for plugins
            strcat(currentFile, "/");
            strcat(currentFile, entry->d_name); //Stitch the file name on
            if ((*pluginCount) > 0) { //If this is not the first plugin
                struct Plugin *newPluginArray = realloc(dest, (*pluginCount + 1) * (sizeof(struct Plugin))); //Realloc array for another plugin
                if(newPluginArray) { //If realloc succeeds replace old array;
                    dest = newPluginArray;
                } else { //If realloc fails then exit and say memory limit reached
                    printf("Error: Too many plugins, memory limit reached");
                    closedir(directory);
                    return dest;
                }
            }
            void *handle;
            handle = dlopen(currentFile, RTLD_NOW); //Gets handle of current library
            if(!handle) { //If handle is invalid skip to next plugin
                continue;
            }

            struct Plugin currentPlugin;
            currentPlugin.handle = handle; //Create a plugin and add all of the properties to it
            *((void **) &currentPlugin.get_plugin_name) = dlsym(handle, "get_plugin_name");
            *((void **) &currentPlugin.get_plugin_desc) = dlsym(handle, "get_plugin_desc");
            *((void **) &currentPlugin.parse_arguments) = dlsym(handle, "parse_arguments");
            *((void **) &currentPlugin.transform_image) = dlsym(handle, "transform_image");
            dest[*pluginCount] = currentPlugin; //Add plugin to the array
            (*pluginCount)++; //Increment plugin counter
        }
    }
    closedir(directory); //Close directory
    return dest; //Return array of plugins
}

//Prints all the loaded plugins
void printLoadedPlugins() {
    int pluginCount = 0;
    struct Plugin *pluginArray = getPluginList(&pluginCount); //get array of plugins and feed pointer for counter variable
    printf("Loaded %d plugin(s)\n", pluginCount);
    unsigned long max = strlen( pluginArray[0].get_plugin_name()); //Get longest Plugin name
    for(int i = 0; i < pluginCount; i++) {
        if(max < strlen(pluginArray[i].get_plugin_name())) { //If current plugin name is longer than current max, make it new max
            max = strlen(pluginArray[i].get_plugin_name());
        }
    }
    for(int i = 0; i < pluginCount; i++) { //Loop through each plugin
        const char* pluginName = pluginArray[i].get_plugin_name();
        for(int j = 0; (size_t) j < (max  - strlen(pluginName)); j++) { //For loop lines up colons for formatting in list using max length value
            printf(" ");
        }
        printf("%s: %s\n", pluginArray[i].get_plugin_name(), pluginArray[i].get_plugin_desc()); //Print name and desc
        dlclose(pluginArray[i].handle); //Close the handle when done
    }
    free(pluginArray); //Free the array when done
}

//Returns index of specified plugin in the provided array, -1 if not found
int getPluginIndex(char* name, int pluginCount, struct Plugin *allPlugins) {
    int index = -1;
    //Get the plugin by name from the plugins loaded
    for(int i = 0; i < pluginCount; i++) {
        if(strcmp((const char *) allPlugins[i].get_plugin_name(), name) == 0) { //If plugin name matches
            index = i; //Put plugin in plugin variable
        }
    }
    if(index == -1) { //If no plugin found return 1
        printf("Error: Plugin not found");
        return index;
    }
    return index;
}

//Frees all of the provided pointers
void handleMemoryLeaks(void *args, struct Plugin *allPlugins, int pluginCount, struct Image *sourceImage, struct Image *newImage) {
    if(sourceImage != NULL) { //If source exists so does new, both must be freed
        free(sourceImage->data);
        free(sourceImage);
        free(newImage->data);
        free(newImage);
    }
    if(args != NULL) { //If args exists, free it
        free(args);
    }
    for(int i = 0; i < pluginCount; i++) { //Free all handles
        dlclose(allPlugins[i].handle);
    }
    free(allPlugins); //Free the array
}

//Runs plugin specified by parameters
int runPlugin(int argc, char* argv[]) {
    int pluginCount = 0; //Amount of plugins
    struct Plugin *allPlugins = getPluginList(&pluginCount); //Get plugin array
    char *name = argv[2]; //Name of plugin we are looking for
    int index = getPluginIndex(name, pluginCount, allPlugins);
    if(index == -1) {
        free(allPlugins);
        return index;
    }
    char* arg[argc-5]; //Trim array down to just plugin arguments
    int argcnew = 0;
    for(int i = 0; i + 5 < argc; i++) {
        arg[i] = argv[i + 5];
        argcnew++;
    }
    void *args = allPlugins[index].parse_arguments(argcnew, arg); //Get arg pointer
    if(args == NULL) {
        printf("Error: Not enough arguments");
        handleMemoryLeaks(args, allPlugins, pluginCount, NULL, NULL);
        return 1;
    }
    struct Image *sourceImage = img_read_png(argv[3]); //load source image
    struct Image *newImage = allPlugins[index].transform_image(sourceImage, args); //Get new image by transforming old one
    img_write_png(newImage, argv[4]); //Write new image with filename specified in args
    //Free everything that was malloced and release handles
    handleMemoryLeaks(NULL, allPlugins, pluginCount, sourceImage, newImage);
    return 0;
}

//Prints the usage of the program
void printUsage() {
    printf("Usage: imgproc <command> [<command args...>]\n");
    printf("Commands are:\n");
    printf(" list\n");
    printf(" exec <plugin> <input img> <output img> [<plugin args...>]\n");
}

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "list") == 0) { //If list, show plugins
        printLoadedPlugins();
    } else if (argc > 1 && strcmp(argv[1], "exec") == 0 && argc >= 5) { //If exec, run the plugin with specified input
        return runPlugin(argc, argv);
    } else { //If output is unrecognized just print the usage
        printUsage();
    }
}
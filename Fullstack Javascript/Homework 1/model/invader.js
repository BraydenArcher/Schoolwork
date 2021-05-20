import Sprite from "./sprite.js";

class Invader extends Sprite {
    constructor(x, y, width, height) {
        const invaderImage = new Image(width, height);
        invaderImage.src = "./assets/invader.png"; //Use invader png as image and use all other variables as given
        super(x, y, width, height, invaderImage);
        this.dx = 1;
        this.dy = 0;
        this.counter = 0; //Get counter for how fast the invaders move back and forth
    }

    move(dx, dy) {
        this.counter++;
        if(this.counter === 10) { //Swap direction every 10 moves for invaders
            this.swap();
        }
        super.move(this.dx, this.dy);
    }
    swap() { //Swaps the direction
        if(this.dx === 1) {
            this.dx = -1;
        } else if(this.dx === -1) {
            this.dx = 1;
        }
        this.counter = 0;
    }
}
export default Invader;
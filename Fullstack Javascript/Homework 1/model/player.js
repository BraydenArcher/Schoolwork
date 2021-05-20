import Sprite from "./sprite.js";
import Bullet from "./bullet.js";

class Player extends Sprite{
    constructor(x, y, width, height) {
        const playerImage = new Image(width, height);
        playerImage.src = "./assets/tank.png";
        super(x, y, width, height, playerImage);
        this.bulletsRemaining = 10;
        this.dx = 0;
        this.bullet = [];
        this.shootAudio = new Audio("./assets/shoot.wav"); //Audio definition
        document.addEventListener("keydown", this.keyDownHandler.bind(this)); //Key handles for keyboard
        document.addEventListener("keyup", this.keyUpHandler.bind(this));
    }

    keyDownHandler(e) { //Keyhandle methods just move players with the arrow keys
        if(e.key === "Right" || e.key === "ArrowRight") {
            this.dx = 7;
        } else if (e.key === "Left" || e.key === "ArrowLeft") {
            this.dx = -7;
        }
    }

    draw(ctx) { //Draws player
        super.draw(ctx);
        for(let i = 0; i < this.bullet.length; i++) { //draws bullets when player is drawn as well
            this.bullet[i].draw(ctx);
            this.bullet[i].move();
        }
    }

    shoot() { //Play audio and spawn bullet
        this.shootAudio.play();
        this.bullet.push(new Bullet(this.x+17, this.y, this.width/3, this.height/3));
    }

    keyUpHandler(e) {
        if(e.key === "Right" || e.key === "ArrowRight") {
            this.dx = 0;
        } else if (e.key === "Left" || e.key === "ArrowLeft") {
            this.dx = 0;
        } else if((e.key === " " || e.key === "Spacebar") && this.bulletsRemaining > 0) { //If spacebar is released,spawn bullets and reduce remaining
            this.bulletsRemaining--;
            this.shoot();
        }
    }
    move(edgeWidth) { //mnove player and then call an edge check
        super.move(this.dx, 0);
        this.checkEdge(edgeWidth);
    }
    checkEdge(edgeWidth) { //verify the player doesnt move off screen
        if(this.x < 0) {
            this.x = 0;
        } else if(this.x + this.width > edgeWidth) {
            this.x = edgeWidth - this.width;
        }
    }
}
export default Player;
import Sprite from "./sprite.js";

class Bullet extends Sprite{
    constructor(x, y ,width, height) {
        const bulletImage = new Image(width, height);
        bulletImage.src = "./assets/missile.png"; //Use bullet image for sprite
        super(x, y, width, height, bulletImage);
        this.dy = -7;
    }
    move() {
        super.move(0, this.dy); //Literally just moves the bullet up
    }
}
export default Bullet;
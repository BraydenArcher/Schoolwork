import Player from "./model/player.js";
import Invader from "./model/invader.js";

const canvas = document.getElementById("myCanvas");
const ctx = canvas.getContext("2d");

const player = new Player(canvas.width / 2 - 25, canvas.height - 60, 50, 50);

const invader = [];
let counter = 50;
let random = Math.floor((Math.random() * 200) + 50);
let gameovere = 0;
let music = new Audio("./assets/music.mpeg"); //Sound variables
let explosion = new Audio("./assets/explosion.wav");
music.loop = true;
document.addEventListener("keydown", logKey);
let setup = 0;

function logKey () { //Checks that user has "Started the game"
  if(setup === 0) {
    music.play();
    setup = 1;
  }
}

let invadersShot = 0;
let bulletsRemaining = player.bulletsRemaining; //Defining variables and setting up canvas
ctx.font = "15px Arial";
ctx.fillStyle = "lightblue";
const gameover = new Event("gameover");


function draw() {
  window.requestAnimationFrame(draw); //Set draw as animation frame
    if(gameovere === 1) {
      return;
    }
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillText("Invaders shot down: " + invadersShot, 10, 15);
    ctx.fillText("Missles remaining: " + bulletsRemaining, 10, 30);
  player.move(canvas.width); //Move player
  player.draw(ctx);
  if(setup === 0) {
    return;
  }
    if (counter === random) { //Generate invaders at random intervals
      counter = 0;
      random = Math.floor((Math.random() * 200) + 50);
      invader.push(new Invader(Math.floor((Math.random() * (canvas.width - 50)) + 50), 10, 50, 50));
      invader[invader.length - 1].dy = Math.floor((Math.random() * 5));
    }
    let invaderLength = invader.length;
    let bulletLength = player.bullet.length;
    for (let i = 0; i < invaderLength; i++) { //Loops to check all conditions of invaders and bullets including whether they collide or not
      if (invader[i] === undefined) {
        continue;
      }
      invader[i].move(canvas.width);
      invader[i].draw(ctx);
      if(invader[i].y > canvas.height) { //Check if invader won
        document.dispatchEvent(gameover);
      }
      for (let j = 0; j < bulletLength; j++) {
        if (player.bullet[j] === undefined) {
          continue;
        }
        if (player.bullet[j].y < 5) { //height check on bullet
          player.bullet.splice(j, 1);
          player.bulletsRemaining++;
        } else if (invader[i].intersects(player.bullet[j])) { //Collision check
          invader.splice(i, 1);
          player.bullet.splice(j, 1);
          player.bulletsRemaining++;
          invadersShot++;
          j = bulletLength;
          explosion.play();
        }
      }
    }
    counter++;
    bulletsRemaining = player.bulletsRemaining;
}

draw();

document.addEventListener("gameover", (_e) => { //Gameover event that stops the game
  music.pause();
  gameovere = 1;
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.fillText("Invaders shot down: " + invadersShot, 10, 15);
  ctx.fillText("Gameover!", 10, 30);
  player.x = (canvas.width / 2 - 25);
  player.draw(ctx);
});
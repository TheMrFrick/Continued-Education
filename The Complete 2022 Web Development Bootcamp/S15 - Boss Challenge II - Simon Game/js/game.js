const buttonColors = ['red', 'blue', 'green', 'yellow'];

const gamePattern = [];
let userClickedPattern = [];

let gameState = false;
let level = 0;

function nextSequence() {
    userClickedPattern = [];
    let randomNumber = Math.round(Math.random() * 3);
    let randomColorChosen = buttonColors[randomNumber];
    level++;
    gamePattern.push(randomColorChosen);
    playGameSequence();
    setH1Level();
}

$('.btn').click((event) => {
    if (gameState) {
        let index = userClickedPattern.length;
        const userChosenColor = event.target.id;
        if (userChosenColor === gamePattern[index]) {
            userClickedPattern.push(userChosenColor);
            animatePress(userChosenColor);
            playSound(userChosenColor);
            if (userClickedPattern.length === gamePattern.length) {
                sleep(500).then(nextSequence);
            }
        }
        else {
            gameOver();
        }
    }
})

$(document).keypress(() => {
    if (!gameState) {
        gameState = true;
        startGame();
    }
})


function playSound(soundName) {
    let audio = new Audio(`./sounds/${soundName}.mp3`);
    audio.play();
}

function animatePress(buttonName) {
    $('#' + buttonName).addClass('pressed');
    setTimeout(() => { $('#' + buttonName).removeClass('pressed') }, 50);
}

function startGame() {
    nextSequence();
}

function setH1Level() {
    $('h1').text(`Level ${level}`);
}

function gameOver() {
    const audio = new Audio('./sounds/wrong.mp3');
    audio.play();
    gameState = false;
    $('h1').text(`Game Over, Score ${level}`);
}

async function playGameSequence() {
    for (color of gamePattern) {
        await animateSequence(color);
    }
}

async function animateSequence(color) {
    $('#' + color).fadeOut(100).fadeIn(100);
    playSound(color);
    await sleep(500);
}

async function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
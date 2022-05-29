let randomNumber1 = Math.floor(Math.random() * 6) + 1;
let randomNumber2 = Math.floor(Math.random() * 6) + 1;

let img1 = document.getElementsByClassName('img1')[0];
let img2 = document.getElementsByClassName('img2')[0];

img1.setAttribute('src', `images/dice${randomNumber1}.png`)
img2.setAttribute('src', `images/dice${randomNumber2}.png`)

let header = document.querySelector('h1');
if(randomNumber1 > randomNumber2){
    header.innerHTML = "Player 1 Wins!"
} 
if(randomNumber1 < randomNumber2){
    header.innerHTML = "Player 2 Wins!"
}
if(randomNumber1 === randomNumber2){
    header.innerHTML = "Tie!"
}
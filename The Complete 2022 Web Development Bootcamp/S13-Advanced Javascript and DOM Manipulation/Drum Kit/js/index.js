// Add Event Listeners
let drums = document.getElementsByClassName('drum')

for (let i = 0; i < drums.length; i++) {
    const drum = drums.item(i);
    drum.addEventListener('click', handleClick);
}


function handleClick() {
    playSound(this.innerHTML);
    buttonAnimation(this.innerHTML);
}

async function playSound(drumButton) {
    switch (drumButton) {
        case 'w':
            let crash = new Audio('./sounds/crash.mp3')
            crash.play();
            break;
        case 'a':
            let kick = new Audio('./sounds/kick-bass.mp3');
            kick.play();
            break;
        case 's':
            let snare = new Audio('./sounds/snare.mp3');
            snare.play();
            break;
        case 'd':
            let tom1 = new Audio('./sounds/tom-2.mp3');
            tom1.play();
            break;
        case 'j':
            let tom2 = new Audio('./sounds/tom-2.mp3');
            tom2.play();
            break;
        case 'k':
            let tom3 = new Audio('./sounds/tom-3.mp3');
            tom3.play();
            break;
        case 'l':
            let tom4 = new Audio('./sounds/tom-4.mp3');
            tom4.play();
            break;
        default:
            break;
    }
}

document.addEventListener('keydown', (event) => {
    playSound(event.key);
    buttonAnimation(event.key);
})

function buttonAnimation(key) {
    let drum = document.querySelector("." + key);
    drum.classList.add('pressed');
    setTimeout(() => { drum.classList.remove('pressed') }, 100);
}
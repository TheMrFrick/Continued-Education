alert('hello');

document.querySelector('h1').innerHTML = 'Goodbye';

let thirdListItem = document.getElementsByClassName('item')[2];
thirdListItem.innerHTML = "Kyle";

let googleListItemLink = document.querySelector('#list .item a');
googleListItemLink.style.color = 'red';

let button = document.getElementsByTagName('button')[0];
button.style.backgroundColor = 'yellow'
function fibonacci(n){
    const arr = [0,1];
    for(let i = 2; i < n; i++){
        arr.push(arr[i-2] + arr[i-1]);
    }
    if(n === 2){
        return [0,1];
    }
    if(n === 1){
        return [0]
    }
    if(n === 0){
        return [];
    }
    return arr;
}

console.log(fibonacci(process.argv[2]));
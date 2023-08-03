console.log("Aur beti");
const myCanvas = document.getElementById("myCanvas");
myCanvas.width = 800;
myCanvas.height = 600;
const ctx = myCanvas.getContext("2d");
console.log(ctx);

let w = null;
let locationOfPixels = null;

function makeEnvironment(...envs){
    return new Proxy(envs, {
        get(target, prop, receiver){
            for(let env of envs){
                if(env.hasOwnProperty(prop)) return env[prop];
            }
            return (...args)=>{console.error("NOT IMPLEMENTED "+prop, args)}
        }
    });
}
WebAssembly.instantiateStreaming(fetch('./wasm.wasm'), {
    "env":  makeEnvironment()
}).then(w0=>{
    w = w0;
    const pixels = w.instance.exports.render();
    const buffer = w.instance.exports.memory.buffer;
    console.log(w);
    console.log(new Uint8Array(buffer, pixels, myCanvas.width*myCanvas.height*4)); // 4 bytes is the size of a single dword (uint32_t)
    const imageData = new ImageData(new Uint8ClampedArray(buffer, pixels, myCanvas.width*myCanvas.height*4), myCanvas.width);
    console.log(imageData);
    ctx.putImageData(imageData, 0, 0);
}).then(()=>{
    console.log(w.instance.exports.render());
    locationOfPixels = w.instance.exports.render();

})
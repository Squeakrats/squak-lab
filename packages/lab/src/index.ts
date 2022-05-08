function main() {
    const canvas: HTMLCanvasElement = document.createElement("canvas");
    canvas.id = "canvas";
    document.body.appendChild(canvas);

    const script: HTMLScriptElement = document.createElement("script");
    script.src = "/packages/lawless/lawless.js"
    document.body.appendChild(script);
}

document.body.onload = main;
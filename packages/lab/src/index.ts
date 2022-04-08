interface Window {
    Module: { canvas: HTMLCanvasElement };
}

function main() {
    window.Module = {
        canvas : document.createElement("canvas")
    };

    document.body.appendChild(window.Module.canvas);

    const script: HTMLScriptElement = document.createElement("script");
    script.src = "/packages/lawless/lawless.js"
    document.body.appendChild(script);
}

document.body.onload = main;
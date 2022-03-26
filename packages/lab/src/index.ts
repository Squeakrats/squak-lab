function main() {
    const script: HTMLScriptElement = document.createElement("script");
    script.src = "/packages/lawless/lawless.js"
    document.body.appendChild(script);
}

document.body.onload = main;
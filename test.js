const wyDecoder = require('./build/Release/wydecoder.node');
const args = process.argv.slice(2);

console.log(" exports: ", wyDecoder);

wyDecoder.load(args[0]);
wyDecoder.start();

while (!wyDecoder.eof()) {
    const obj = wyDecoder.frame(10);

    if (obj.pts) {
        console.log("Frame: " + obj.width + "x" + obj.height + " pts: " + obj.pts + 
            " dataY:" + obj.lum_data.byteLength +
            " dataU:" + obj.u_data.byteLength +
            " dataV:" + obj.v_data.byteLength
        );
        // const buf = new Uint8Array(obj.data);
        // console.log("Val: " + buf[256] + " " + buf[512] + " "  + buf[1024]);
    } else
        console.log("....waiting...");
}
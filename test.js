// Compress
let data = [0.5678, 1.14567, 1.11111, 7.96969, 6.96969];
let typedData = new Float64Array(data);
let data_p = Module._exported_malloc(
  typedData.length * typedData.BYTES_PER_ELEMENT,
);
Module.HEAPF64.set(typedData, data_p / typedData.BYTES_PER_ELEMENT);

let compressed = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
let typedCompressed = new Uint8Array(compressed);
let compressed_p = Module._exported_malloc(
  typedCompressed.length * typedCompressed.BYTES_PER_ELEMENT,
);
Module.HEAP8.set(
  typedCompressed,
  compressed_p / typedCompressed.BYTES_PER_ELEMENT,
);

let outSize = [0];
let typedOutSize = new Uint32Array(outSize);
let outSize_p = Module._exported_malloc(
  typedOutSize.length * typedOutSize.BYTES_PER_ELEMENT,
);
Module.HEAP32.set(typedOutSize, outSize_p / typedOutSize.BYTES_PER_ELEMENT);

Module._sz_compress_f64(data_p, compressed_p, outSize_p);
outSize = Module.HEAP32.subarray(
  outSize_p / typedOutSize.BYTES_PER_ELEMENT,
  outSize_p / typedOutSize.BYTES_PER_ELEMENT + typedOutSize.length,
);
compressed = Module.HEAP8.subarray(
  compressed_p / typedCompressed.BYTES_PER_ELEMENT,
  compressed_p / typedCompressed.BYTES_PER_ELEMENT + typedCompressed.length,
);
console.log(compressed);

// Decompress
Module._sz_decompress_f64(compressed_p, outSize[0], data_p);
data = Module.HEAPF64.subarray(
  data_p / typedData.BYTES_PER_ELEMENT,
  data_p / typedData.BYTES_PER_ELEMENT + typedData.length,
);
console.log(data);

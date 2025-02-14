// This one is filled out by a script on mount
let compressedArray = [];

// hardcoded double vector length for now
let originalLength = 1562;

// malloc and memcpy compressed array into wasm memory
let compressed_p = Module._exported_malloc(compressedArray.length);
Module.HEAP8.set(compressedArray, compressed_p);

// malloc for uncompressed doubles
let new_data_p = Module._exported_malloc(originalLength * 8);

// Run decompression
Module._sz_decompress_f64(
  compressed_p,
  compressedArray.length,
  new_data_p,
  originalLength,
);

// Retrieve new data
let new_data = Module.HEAPF64.subarray(
  new_data_p / 8,
  new_data_p / 8 + originalLength,
);

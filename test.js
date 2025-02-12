let data = [0.5678, 1.14567, 1.11111, 7.96969, 6.96969];
let typedData = new Float64Array(data);
let pointer = Module._exported_malloc(
  typedData.length * typedData.BYTES_PER_ELEMENT,
);
Module.HEAPF64.set(typedData, pointer / typedData.BYTES_PER_ELEMENT);
Module._comp_test(pointer);
let newData = Module.HEAPF64.subarray(
  pointer / typedData.BYTES_PER_ELEMENT,
  pointer / typedData.BYTES_PER_ELEMENT + typedData.length,
);
console.log(newData);

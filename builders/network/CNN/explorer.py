import tensorflow as tf

model = tf.lite.Interpreter('lite.tflite')
model.allocate_tensors()
board = model.get_input_details()[0]
color = model.get_input_details()[1]
outp = model.get_output_details()

print(outp)
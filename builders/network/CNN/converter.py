import tensorflow as tf
import sys

converter = tf.lite.TFLiteConverter.from_keras_model(tf.keras.models.load_model(sys.argv[1]))
tflite_model = converter.convert()

# Save the model.
with open('models/lite.tflite', 'wb') as f:
  f.write(tflite_model)
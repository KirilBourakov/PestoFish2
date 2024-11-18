import tensorflow as tf
import sys

model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(64, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
        
        tf.keras.layers.Flatten(),

        tf.keras.layers.Dense(2048, activation='softmax'),
        tf.keras.layers.Dense(2048, activation='softmax'),
        tf.keras.layers.Dense(1)
    ])
model.load_weights(sys.argv[1])
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open('lite.tflite', 'wb') as f:
    f.write(tflite_model)
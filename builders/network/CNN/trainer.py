import sys
import tensorflow as tf
import time
from utils import read

def main():
    filepath = sys.argv[1]
    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
        tf.keras.layers.Conv2D(16, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(32, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(64, (2,2), activation='relu'),

        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(64),
        tf.keras.layers.Dense(32),
        tf.keras.layers.Dense(1)
    ])
    model.compile(optimizer='adam', loss='mae', metrics=['mae'])
    model.fit(read(filepath), epochs=10, steps_per_epoch=4000000)

    model.evaluate(read(filepath), steps_per_epoch=4000000)
    
    model.save(f"models/model_{time.ctime(time.time())}.keras".replace(":", "."))

if __name__ == '__main__':
    main()
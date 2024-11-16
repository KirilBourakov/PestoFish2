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
        # tf.keras.layers.Dense(32),
        tf.keras.layers.Dense(1)
    ]) 

    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="models/small_adam.weights.h5",
        monitor='loss',
        mode='min',
        save_best_only=True,
        save_weights_only=True,
    )

    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.0001), loss='mse', metrics=['mae'])
    model.fit(read(filepath, 60), epochs=10, steps_per_epoch=200000, callbacks=[model_checkpoint_callback])

    model.evaluate(read(filepath, 1), steps=10000)
    
    model.save(f"models/model_{time.ctime(time.time())}.keras".replace(":", "."))

if __name__ == '__main__':
    main()
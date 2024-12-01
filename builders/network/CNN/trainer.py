import sys
import tensorflow as tf
import time
from sklearn.model_selection import train_test_split
import numpy as np
import csv
from utils import read_other as read

def main():
    # model = tf.keras.Sequential([
    #     tf.keras.layers.Conv2D(96, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
        
    #     tf.keras.layers.Flatten(),

    #     tf.keras.layers.Dense(2048),
    #     tf.keras.layers.Dense(2048),

    #     tf.keras.layers.Dense(1)
    # ])

    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="models/6x8x8_non_convo_full.keras",
        monitor='loss',
        mode='min',
    )
    # model.load_weights("models/6x8x8_full.weights.h5")
    # model.compile(optimizer='adam', loss='mse', metrics=['mae'])
    model = tf.keras.models.load_model("models/conv96,256,384,192,1 (epoch 29).keras")

    for j in range(20):
        print('epoch:', j+1)
        for i in range(7):
            print('part:', i+1)
            features, evals = read(f"{sys.argv[1]}/{i+1}.csv")
            model.fit(features, evals, epochs=1, callbacks=[model_checkpoint_callback], validation_split=0.1)

    # features, evals = read(f"{sys.argv[1]}/{7}.csv")
    # model.evaluate(features, evals)

    model.save(f"model_{time.ctime(time.time())}.keras".replace(":", "."))



if __name__ == '__main__':
    main()
from sklearn.model_selection import train_test_split
import tensorflow as tf
import sys
import time
from lichess_read import read

def main():

    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
        tf.keras.layers.Conv2D(16, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(32, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(64, (2,2), activation='relu'),

        tf.keras.layers.Flatten(),

        tf.keras.layers.Dense(129),
        tf.keras.layers.Dense(1)
    ])
    model.compile(optimizer='adam', loss='mae', metrics=['mae'])

    location, cap, epoch_max = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])

    curr_epoch = 1

    while curr_epoch <= epoch_max:
        for file in range(cap):
            features, evaluation = read(f"{location}/{file+1}.csv")
            model.fit(features, evaluation, batch_size=1750000)
        curr_epoch += 1

    features, evaluation = read(f"{location}/{cap+1}.csv")
    model.evaluate(features, evaluation)
    
    model.save(f"model_{time.ctime(time.time())}.keras".replace(":", "."))

if __name__ == '__main__':
    main()
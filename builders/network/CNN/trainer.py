from sklearn.model_selection import train_test_split
import tensorflow as tf
import sys
import time
from reader import read

def main():
    features, evals = read(sys.argv[1])

    training_features, testing_features, training_eval, testing_eval = train_test_split(features, evals, test_size=0.3)
    print('h')
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
    print('d')
    model.compile(optimizer='adam', loss='mae', metrics=['mae'])
    model.fit(training_features, training_eval, epochs=10)
    model.evaluate(testing_features, testing_eval)
    print('c')
    # model.save(f"model_{time.ctime(time.time())}.keras".replace(":", "."))

if __name__ == '__main__':
    main()
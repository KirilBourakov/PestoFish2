import keras_tuner as kt
import tensorflow as tf
from utils import read_other

def make_model(hp):
    model = tf.keras.Sequential()

    # sequential layer
    conv_nodes = hp.Int('conv_nodes', min_value=64, max_value=128, step=32)
    model.add(tf.keras.layers.Conv2D(conv_nodes, (3,3), input_shape=(6,8,8), activation='relu', padding="same"))
    model.add(tf.keras.layers.Flatten())

    # hidden layer
    for i in range(hp.Choice('num_layers', [1,2,3])):
        model.add(tf.keras.layers.Dense(hp.Int(f'nodes_{i}', min_value=128, max_value=512, step=64)))
    
    # output
    model.add(tf.keras.layers.Dense(1))

    # compile
    rate = hp.Choice('learning_rate', values=[0.001, 0.0001])
    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=rate),
                loss='mse',
                metrics=['mae'])
    return model

def main():
    hyper_tuner = kt.Hyperband(
        make_model,
        objective='val_loss',
        factor=2,
        directory="../data",
        project_name="tuner"
    )
    print('reading...')
    boards, evals = read_other('../data/out/1.csv')

    stop = tf.keras.callbacks.EarlyStopping(monitor='val_loss', patience=2)
    hyper_tuner.search(boards, evals, validation_split=0.2, callbacks=[stop])
    best = hyper_tuner.get_best_hyperparameters()[0]
    
    model = hyper_tuner.hypermodel.build(best)
    model.save('best.keras')

if __name__ == '__main__':
    main()
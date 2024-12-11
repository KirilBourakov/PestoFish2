import tensorflow as tf
import sys
from utils import read_other_double as read

# currently not impressive
def create_model():
    position = tf.keras.Input(shape=(6,8,8), dtype="float32", name='pos')
    color_move = tf.keras.Input(shape=(1,), dtype="int16", name='color_move')


    # 2d convolution
    conv =  tf.keras.layers.Conv2D(96, (3,3), activation='relu', padding="same", name='conv')(position)
    conv = tf.keras.layers.Flatten()(conv)

    x = tf.keras.layers.concatenate([conv, color_move])
    x = tf.keras.layers.Dense(256)(x)
    x = tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)
    x = tf.keras.layers.Dense(256)(x)
    tf.keras.layers.LeakyReLU()(x)

    output = tf.keras.layers.Dense(1, name='output')(x)

    model = tf.keras.Model(inputs=[position, color_move], outputs=output)
    tf.keras.utils.plot_model(model, "graph.png", show_shapes=True)
    return model

def train(model, path):
    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="training/double.keras",
        monitor='loss',
        mode='min',
    )
    model.compile(optimizer='adam', loss='mse', metrics=['mae'])


    for j in range(1):
        print('epoch:', j+1)
        for i in range(7):
            print('part:', i+1)
            board, move, evals = read(f"{path}/{i+1}.csv")
            model.fit(x=[board, move], y=evals, epochs=1, callbacks=[model_checkpoint_callback], validation_split=0.1)

if __name__ == "__main__":
    # model = create_model()
    model = tf.keras.models.load_model("models/double.keras")
    train(model, sys.argv[1])
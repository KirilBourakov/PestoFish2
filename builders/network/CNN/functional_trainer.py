import tensorflow as tf
import sys
from utils import read_other as read 

# currently not impressive
def create_model():
    inputs = tf.keras.Input(shape=(6,8,8))

    # 2d convolution
    twod_conv =  tf.keras.layers.Conv2D(96, (3,3), activation='relu', padding="same")(inputs)
    twod_conv = tf.keras.layers.Flatten()(twod_conv)

    # 
    h_line = tf.keras.layers.Conv2D(10, (1,8), activation='relu', padding="same")(inputs)
    h_line = tf.keras.layers.Flatten()(h_line)

    # 
    v_line = tf.keras.layers.Conv2D(10, (1,8), activation='relu', padding="same")(inputs)
    v_line = tf.keras.layers.Flatten()(v_line)

    x = tf.keras.layers.concatenate([v_line, h_line, twod_conv])
    
    # network
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)
    x = tf.keras.layers.Dense(128)(x)

    output = tf.keras.layers.Dense(1)(x)

    model = tf.keras.Model(inputs=[inputs], outputs=[output])
    tf.keras.utils.plot_model(model, "graph.png", show_shapes=True)
    return model

def train(model, path):
    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="models/96,128x10 (relu),1.keras",
        monitor='loss',
        mode='min',
    )
    model.compile(optimizer='adam', loss='mse', metrics=['mae'])
    for j in range(20):
        print('epoch:', j+1)
        for i in range(7):
            print('part:', i+1)
            features, evals = read(f"{path}/{i+1}.csv")
            model.fit(features, evals, epochs=1, callbacks=[model_checkpoint_callback], validation_split=0.1)

if __name__ == "__main__":
    model = create_model()
    train(model, sys.argv[1])
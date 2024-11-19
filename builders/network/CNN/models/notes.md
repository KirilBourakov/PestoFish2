neural netsize seems to not have large impact on running times after converted to a lite model

6x8x8_full (restart 104).weights.h5 
- trained out till 104 on the lichess database. Stabalized at a loss of 700k, with 2.5 centipawn error. 
- 1ms/step.
- Likely underfitting
- tf.keras.layers.Dense(32, activation='softmax'),
    tf.keras.layers.Dense(128, activation='softmax'),
    tf.keras.layers.Dense(1)

6x8x8_full (restart 7)
 - train until 7 on lichess database. Ended with loss: 974420.0625 - mae: 234.6449 
 - Training cut short, wanted to try on with only 1 hidden layer, but more nodes - (makes more intuative sense)
 - 4ms/step
 - tf.keras.layers.Conv2D(64, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
    tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(256, activation='softmax'),
        tf.keras.layers.Dense(128, activation='softmax'),
        tf.keras.layers.Dense(1)

6x8x8_full (restart 54)
- Stabalized around 2.5-2.7 centipawn error.
- Training cut short as my computer bluescreened
- Likely still underfitting
- tf.keras.layers.Conv2D(64, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(512, activation='softmax'),
    tf.keras.layers.Dense(1)
    
6x8x8_full (restart 28)
- Struggled to get below loss of 1mil.
- tf.keras.layers.Conv2D(64, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(2048, activation='softmax'),
    tf.keras.layers.Dense(2048, activation='softmax'),
    tf.keras.layers.Dense(1)
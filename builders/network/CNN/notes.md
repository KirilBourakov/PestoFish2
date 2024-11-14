## 1st run
model:
    tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
    tf.keras.layers.Conv2D(16, (3,3), activation='relu'),
    tf.keras.layers.Conv2D(32, (3,3), activation='relu'),
    tf.keras.layers.Conv2D(64, (2,2), activation='relu'),

    tf.keras.layers.Flatten(),

    tf.keras.layers.Dense(64),
    tf.keras.layers.Dense(1)
result:
    18750/18750 ━━━━━━━━━━━━━━━━━━━━ 14s 731us/step - loss: 34.1212 - mae: 34.1212 
Note: ran on first 2 mill items in dataset. Full dataset takes up too much space in RAM.

## 2nd run
model:
    tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
    tf.keras.layers.Conv2D(32, (3,3), activation='relu'),

    tf.keras.layers.Flatten(),

    tf.keras.layers.Dense(64),
    tf.keras.layers.Dense(1)
result: 
    18750/18750 ━━━━━━━━━━━━━━━━━━━━ 10s 537us/step - loss: 37.5452 - mae: 37.5452

## 3nd run
model:
    tf.keras.layers.Conv2D(14, (3,3), activation='relu', input_shape=(8,8,6)),
    tf.keras.layers.Conv2D(3, (3,3), activation='relu'),

    tf.keras.layers.Flatten(),

    tf.keras.layers.Dense(1)
result: 
    18750/18750 ━━━━━━━━━━━━━━━━━━━━ 10s 533us/step - loss: 39.1153 - mae: 39.1153

## 4th run
model:
    tf.keras.layers.Conv2D(4, (3,3), activation='relu', input_shape=(8,8,6)),
    tf.keras.layers.Conv2D(2, (3,3), activation='relu'),

    tf.keras.layers.Flatten(),

    tf.keras.layers.Dense(1)
result: 
    18750/18750 ━━━━━━━━━━━━━━━━━━━━ 9s 450us/step - loss: 40.2423 - mae: 40.242360
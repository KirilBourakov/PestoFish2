# https://www.kaggle.com/datasets/ronakbadhe/chess-evaluations
## 1st run
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(12))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='mae', metrics=['accuracy']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 77s 378us/step - accuracy: 0.0358 - loss: 295.2667

## 2nd run
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(12))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 90s 443us/step - MAPE: 82898.4219 - loss: 82898.4219 - mae: 300.7828

## 3nd run
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 87s 429us/step - MAPE: 680406.1250 - loss: 680406.1250 - mae: 300.9248 0303

## 4th run
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 119s 589us/step - MAPE: 95296.2422 - loss: 95296.2422 - mae: 300.3830

## 5th run (uses rev2 map)
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 123s 605us/step - MAPE: 1154506.8750 - loss: 1154506.8750 - mae: 300.5866  

## 6th run
**pre-compile**
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1087))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    note: .5 split
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 167s 821us/step - MAPE: 278944.3750 - loss: 278944.3750 - mae: 300.6656

## 7th run (uses rev2 transformer)
**pre-compile**
    model.add(tf.keras.layers.Dense(64, input_shape=(64,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 117s 579us/step - MAPE: 34125.0234 - loss: 34125.0234 - mae: 300.1754 


## 8th run (uses rev2 transformer & rev2 map)
**pre-compile**
    model.add(tf.keras.layers.Dense(64, input_shape=(64,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='MAPE', metrics=['MAPE', 'mae']
**fit**
    epoch 10
**result**
    <!-- NOT NOTED -->

## 9th run (uses rev2 transformer & rev2 map)
Notes: loss nearly identical at epoch 1 as at epoch 10
**pre-compile**
    model.add(tf.keras.layers.Dense(64, input_shape=(64,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='mse', metrics=['mse', 'mae']
**fit**
    epoch 10
**result**
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 9419s 47ms/step - loss: 635993.0625 - mae: 318.6691 - mse: 635993.0625 

## 10th run (uses rev2 transformer & rev2 map)
**pre-compile**
    model.add(tf.keras.layers.Dense(64, input_shape=(64,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
**compile**
    optimizer='adam', loss='mae', metrics=['mae']
**fit**
    epoch 10
**result**
    202470/202470 ━━━━━━━━━━━━━━━━━━━━ 122s 604us/step - loss: 296.9246 - mae: 296.9246
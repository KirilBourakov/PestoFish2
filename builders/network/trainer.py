import csv
import tensorflow as tf
from sklearn.model_selection import train_test_split
import numpy as np
import sys
from code_maps import code_map_rev2 as code_map
from transformers import transform_rev_2 as transform

# TODO: try 2 models, one to classify who has the advantage, the other to estimate the value of the adv
def main():
    with open(sys.argv[1], newline='') as file:
        reader = csv.DictReader(file)
        features = []
        eval = []
        for row in reader:
            features.append(transform(row, code_map))
            # NOTE: this is incorrect; moves that start with a # should be purged, as they show the number of moves until checkmate, not an eval
            eval.append(int(row['Evaluation'].replace('#', '')))
    eval = np.array(eval)
    features = np.array(features)
    model(eval, features)

def model(eval, features):
    training_features, testing_features, training_eval, testing_eval = train_test_split(features, eval, test_size=0.5)    

    model = tf.keras.models.Sequential()
    model.add(tf.keras.layers.Dense(64, input_shape=(64,)))
    model.add(tf.keras.layers.Dense(135))
    model.add(tf.keras.layers.Dense(271))
    model.add(tf.keras.layers.Dense(543))
    model.add(tf.keras.layers.Dense(1))
    model.compile(optimizer='adam', loss='mae', metrics=['mae'])
    model.fit(training_features, training_eval, epochs=10)
    model.evaluate(testing_features, testing_eval)

    model.save('run10.keras')

if __name__ == "__main__":
    main()
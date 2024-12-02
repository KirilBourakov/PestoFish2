import os, sys
import tensorflow as tf
from utils import read_lichess

def main(folder, data):
    models = os.listdir(path=folder)
    features, evals = read_lichess(data)

    for model_name in models:
        if model_name.count('.md') > 0:
            continue
        print(f'----------------------------- {model_name} -------------------------------')
        model = tf.keras.models.load_model(os.path.join(folder, model_name))
        model.evaluate(features, evals)

# best so far: 96,256x7 (relu), 1 (epoch 12)
if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
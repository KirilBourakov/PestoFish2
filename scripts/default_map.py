def generate_start():
    li = []
    for i in range(8):
        li.append([0]*8)
    return li

heat_hash = {
    # endgame
    True: {
        # black piece heatmap
        "B": {
            "P": generate_start(),
            "K": generate_start(),
            "Q": generate_start(),
            "R": generate_start(),
            "B": generate_start(),
            "N": generate_start(),
        },
        # white piece heatmap
        "W": {
            "P": generate_start(),
            "K": generate_start(),
            "Q": generate_start(),
            "R": generate_start(),
            "B": generate_start(),
            "N": generate_start(),
        }   
    },
    # Middle game
    False: {
        # black piece heatmap
        "B": {
            "P": generate_start(),
            "K": generate_start(),
            "Q": generate_start(),
            "R": generate_start(),
            "B": generate_start(),
            "N": generate_start(),
        },
        # white piece heatmap
        "W": {
            "P": generate_start(),
            "K": generate_start(),
            "Q": generate_start(),
            "R": generate_start(),
            "B": generate_start(),
            "N": generate_start(),
        }   
    }
}

import csv
import json
import sys

# https://docs.google.com/spreadsheets/d/1AmmuLvcl9kZe4-kctf6OUijyuCAsmQgsgGxh5M7PU9g/edit#gid=0

def parse_config_csv(file_input):
    config = {"clusters": {}}
    if isinstance(file_input, str):
        with open(file_input, newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                cluster_id = int(row['clusterId'])
                if cluster_id not in config['clusters']:
                    config['clusters'][cluster_id] = {
                        "role": row['role'],
                        "nodes": []
                    }
                node_data = {
                    "nodeId": int(row['nodeId']),
                    "ledCount": int(row['ledCount']),
                    "ringCoordinates": [int(x) for x in row['ringCoordinates'].split(',') if x],
                    "cubeCoordinates": [int(x) for x in row['cubeCoordinates'].split(',') if x],
                    "cartesian2dCoordinates": [float(x) for x in row['cartesian2dCoordinates'].split(',') if x]
                }
                config['clusters'][cluster_id]['nodes'].append(node_data)
    else:
        reader = csv.DictReader(file_input)
        # Similar processing logic for file-like object

    return config

if __name__ == "__main__":
    if len(sys.argv) > 2:
        input_file_path = sys.argv[1]
        output_file_path = sys.argv[2]
        config = parse_config_csv(input_file_path)

        with open(output_file_path, 'w') as json_file:
            json.dump(config, json_file, indent=4)
    else:
        print("Error: Missing required arguments. Usage: parse_config_csv.py <input_csv_path> <output_json_path>")
        sys.exit(1)

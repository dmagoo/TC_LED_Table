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
                    "touchSensorGPIOPin": int(row['touchSensorGPIOPin']),
                    "ringCoordinates": [int(x) for x in row['ringCoordinates'].split(',') if x],
                    "cubeCoordinates": [int(x) for x in row['cubeCoordinates'].split(',') if x],
                    "cartesian2dCoordinates": [float(x) for x in row['cartesian2dCoordinates'].split(',') if x]
                }
                config['clusters'][cluster_id]['nodes'].append(node_data)
    else:
        reader = csv.DictReader(file_input)
        # Similar processing logic for file-like object

    return config

def generate_cpp_code(config):
    cpp_code = "#include \"make_cluster_config.h\"\n\n"
    cpp_code += "std::vector<ClusterConfig> makeClusterConfigs() {\n"
    cpp_code += "    return {\n"
    for cluster_id, cluster in config['clusters'].items():
        cpp_code += f"        ClusterConfig({cluster_id}, makeNodeConfigs({cluster_id})),\n"
    cpp_code += "    };\n"
    cpp_code += "}\n\n"

    cpp_code += "// allow a universe of a single cluster for unit testing\n"
    cpp_code += "std::vector<ClusterConfig> makeClusterConfig(int clusterId) {\n"
    cpp_code += "    return {\n"
    cpp_code += "        ClusterConfig(clusterId, makeNodeConfigs(clusterId)),\n"
    cpp_code += "    };\n"
    cpp_code += "}\n\n"

    cpp_code += "std::vector<NodeConfig> makeNodeConfigs(int clusterId) {\n"
    cpp_code += "    switch(clusterId) {\n"
    for cluster_id, cluster in config['clusters'].items():
        cpp_code += f"    case {cluster_id}:\n"
        cpp_code += "        return {\n"
        nodes = cluster['nodes']
        for i, node in enumerate(nodes):
            nodeId = node['nodeId']
            ledCount = node['ledCount']
            ringCoords = ', '.join(str(x) for x in node['ringCoordinates'])
            cubeCoords = ', '.join(str(x) for x in node['cubeCoordinates'])
            cartesian2dCoords = ', '.join(str(x) for x in node['cartesian2dCoordinates'])
            cpp_code += f"            NodeConfig({nodeId}, {ledCount},\n"
            cpp_code += f"                RingCoordinate({ringCoords}),\n"
            cpp_code += f"                CubeCoordinate({cubeCoords}),\n"
            cpp_code += f"                Cartesian2dCoordinate({cartesian2dCoords}))"
            if i < len(nodes) - 1:
                cpp_code += ","
            cpp_code += "\n"
        cpp_code += "        };\n"
    cpp_code += "    }\n"
    cpp_code += "    return {};\n"
    cpp_code += "}\n"
    return cpp_code

if __name__ == "__main__":
    if len(sys.argv) > 2:
        input_file_path = sys.argv[1]
        output_file_path = sys.argv[2]
        config = parse_config_csv(input_file_path)
        
        # Change output mode based on file extension
        if output_file_path.endswith('.json'):
            with open(output_file_path, 'w') as json_file:
                json.dump(config, json_file, indent=4)
        elif output_file_path.endswith('.cpp'):
            cpp_code = generate_cpp_code(config)
            with open(output_file_path, 'w') as cpp_file:
                cpp_file.write(cpp_code)
        else:
            print("Error: Unsupported output file format. Please use .json or .cpp extension.")
            sys.exit(1)
    else:
        print("Error: Missing required arguments. Usage: parse_config_csv.py <input_csv_path> <output_json_path>")
        sys.exit(1)

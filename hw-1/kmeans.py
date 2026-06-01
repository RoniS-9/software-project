import sys

def main():

    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("An Error Has Occurred")
        sys.exit(1)
        
    try:
        K = int(sys.argv[1])
    except ValueError:
        print("An Error Has Occurred")
        sys.exit(1)
        
    if K <= 1:
        print("Incorrect number of clusters!")
        sys.exit(1)
        
    if len(sys.argv) == 3:
        try:
            max_iter = int(sys.argv[2])
        except ValueError:
            print("An Error Has Occurred")
            sys.exit(1)
            
        if max_iter <= 1 or max_iter >= 800:
            print("Incorrect maximum iteration!")
            sys.exit(1)
    else:
        max_iter = 400

    datapoints = []
    
    for line in sys.stdin:
        line = line.strip() 
        if not line:
            continue
            
        try:
            split_line = line.split(',')
            point = []
            for x in split_line:
                point.append(float(x))
            datapoints.append(point)
        except ValueError:
            print("An Error Has Occurred")
            sys.exit(1)

    N = len(datapoints)
    
    if N == 0:
        print("An Error Has Occurred")
        sys.exit(1)
        
    if K >= N:
        print("Incorrect number of clusters!")
        sys.exit(1)

    centroids = []
    for i in range(K):
        current_centroid = []
        for x in datapoints[i]:
            current_centroid.append(x)
        centroids.append(current_centroid)

    epsilon = 0.001
    iteration_number = 0
    max_delta = epsilon + 1.0 
    
    while (max_delta >= epsilon) and (iteration_number < max_iter):
        
        clusters = []
        for i in range(K):
            clusters.append([])
            
        for point in datapoints:
            min_dist = float('inf')
            cluster_id = 0
            
            for i in range(K):
                dist = euclidean_distance(point, centroids[i])
                if dist < min_dist:
                    min_dist = dist
                    cluster_id = i
                    
            clusters[cluster_id].append(point)
            
        new_centroids = []
        d = len(datapoints[0])
        
        for i in range(K):
            cluster_points = clusters[i]
            new_centroid = []
            
            for j in range(d):
                sum_xi = 0.0
                for p in cluster_points:
                    sum_xi += p[j]
                new_centroid.append(sum_xi / len(cluster_points))
                
            new_centroids.append(new_centroid)
            
        max_delta = 0.0
        for i in range(K):
            delta = euclidean_distance(centroids[i], new_centroids[i])
            if delta > max_delta:
                max_delta = delta
                
        centroids = new_centroids
        iteration_number += 1

    for centroid in centroids:
        formatted_centroid = []
        for coord in centroid:
            formatted_coord = "%.4f" % coord 
            formatted_centroid.append(formatted_coord)
        print(",".join(formatted_centroid))


def euclidean_distance(point1, point2):
    sum_sq = 0.0
    for i in range(len(point1)):
        sum_sq += (point1[i] - point2[i]) ** 2
    return sum_sq ** 0.5

if __name__ == "__main__":
    main()
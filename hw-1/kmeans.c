#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct cord
{
    double value;
    struct cord *next;
};

struct vector
{
    struct vector *next;
    struct cord *cords;
};

double euclidean_distance(double *p1, double *p2, int d);

int main(int argc, char *argv[]) {
    int K;
    int max_iter;
    
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    double n;
    char c;
    
    int N = 0;
    int d = 0;
    int current_d = 0;

    double **data;
    struct vector *temp_vec;
    struct cord *temp_cord;
    int i, j;

    double **centroids;

    int iter = 0;
    int is_changed = 1;
    double EPSILON = 0.001;
    double **cluster_sums;
    int *cluster_counts;
    int p, cluster_idx, min_idx;
    double min_dist, dist, new_val, diff;

    if (argc < 2 || argc > 3) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    K = atoi(argv[1]);
    if (K <= 1) {
        printf("Incorrect number of clusters!\n");
        exit(1);
    }

    if (argc == 3) {
        max_iter = atoi(argv[2]);
        if (max_iter <= 1 || max_iter >= 800) {
            printf("Incorrect maximum iteration!\n");
            exit(1);
        }
    } else {
        max_iter = 400;
    }

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2)
    {
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            
            N++; 
            current_d++;
            if (d == 0) {
                d = current_d; 
            }
            current_d = 0;
            continue;
        }
        
        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        current_d++;
    }
    
    if (K >= N) {
        printf("Incorrect number of clusters!\n");
        curr_vec = head_vec;
        while (curr_vec != NULL) {
            temp_vec = curr_vec;
            curr_cord = curr_vec->cords;
            while (curr_cord != NULL) {
                temp_cord = curr_cord;
                curr_cord = curr_cord->next;
                free(temp_cord);
            }
            curr_vec = curr_vec->next;
            free(temp_vec);
        }        
        exit(1);
    }

    data = malloc(N * sizeof(double *));
    if (data == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    curr_vec = head_vec;
    for (i = 0; i < N; i++) {
        data[i] = malloc(d * sizeof(double));
        if (data[i] == NULL) {
            printf("An Error Has Occurred\n");
            exit(1);
        }

        curr_cord = curr_vec->cords;
        for (j = 0; j < d; j++) {
            data[i][j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
       
    curr_vec = head_vec;
    while (curr_vec != NULL) {
        temp_vec = curr_vec;
        curr_cord = curr_vec->cords;
        
        while (curr_cord != NULL) {
            temp_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(temp_cord); 
        }
        
        curr_vec = curr_vec->next;
        free(temp_vec);
    }

    centroids = malloc(K * sizeof(double *));
    if (centroids == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    for (i = 0; i < K; i++) {
        centroids[i] = malloc(d * sizeof(double));
        if (centroids[i] == NULL) {
            printf("An Error Has Occurred\n");
            exit(1);
        }
        
        for (j = 0; j < d; j++) {
            centroids[i][j] = data[i][j];
        }
    }

    cluster_sums = malloc(K * sizeof(double *));
    if (cluster_sums == NULL) { printf("An Error Has Occurred\n"); exit(1); }
    
    cluster_counts = malloc(K * sizeof(int));
    if (cluster_counts == NULL) { printf("An Error Has Occurred\n"); exit(1); }
    
    for (i = 0; i < K; i++) {
        cluster_sums[i] = malloc(d * sizeof(double));
        if (cluster_sums[i] == NULL) { printf("An Error Has Occurred\n"); exit(1); }
    }

    while (iter < max_iter && is_changed) {
        is_changed = 0;

        for (i = 0; i < K; i++) {
            cluster_counts[i] = 0;
            for (j = 0; j < d; j++) {
                cluster_sums[i][j] = 0.0;
            }
        }

        for (p = 0; p < N; p++) {
            min_dist = -1.0;
            min_idx = 0;

            for (cluster_idx = 0; cluster_idx < K; cluster_idx++) {
                dist = euclidean_distance(data[p], centroids[cluster_idx], d);
                
                if (min_dist == -1.0 || dist < min_dist) {
                    min_dist = dist;
                    min_idx = cluster_idx;
                }
            }

            cluster_counts[min_idx]++;
            for (j = 0; j < d; j++) {
                cluster_sums[min_idx][j] += data[p][j];
            }
        }

        for (cluster_idx = 0; cluster_idx < K; cluster_idx++) {
            if (cluster_counts[cluster_idx] > 0) {
                dist = 0.0; 
                for (j = 0; j < d; j++) {
                    new_val = cluster_sums[cluster_idx][j] / cluster_counts[cluster_idx];
                    diff = new_val - centroids[cluster_idx][j];
                    dist += diff * diff;
                    centroids[cluster_idx][j] = new_val;
                }
                
                if (sqrt(dist) >= EPSILON) {
                    is_changed = 1;
                }                
            }
        }
        iter++;
    }

    for (i = 0; i < K; i++) {
        for (j = 0; j < d; j++) {
            printf("%.4f", centroids[i][j]);
            if (j < d - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    for (i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);

    for (i = 0; i < K; i++) {
        free(centroids[i]);
        free(cluster_sums[i]);
    }
    free(centroids);
    free(cluster_sums);
    free(cluster_counts);

    return 0;
}

double euclidean_distance(double *p1, double *p2, int d) {
    double sum = 0.0;
    double diff;
    int i;
    
    for (i = 0; i < d; i++) {
        diff = p1[i] - p2[i];
        sum += diff * diff;
    }
    
    return sqrt(sum);
}
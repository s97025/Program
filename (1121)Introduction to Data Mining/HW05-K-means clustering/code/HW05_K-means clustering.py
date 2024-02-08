import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs
from sklearn.metrics import silhouette_score


# 生成資料
X, y = make_blobs(n_samples=300, centers=4, random_state=42)
# X, y = make_blobs(n_samples=300, centers=4, random_state=0, cluster_std=1)


print(X, y)


# 繪製散點圖
plt.scatter(X[:, 0], X[:, 1], c=y, cmap='viridis', edgecolors='k')
plt.title('Generated Data with make_blobs(by kiwi_tech)')
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.show()


# 使用手肘法找到最佳的集群數
wcss = []
for i in range(1, 11):
    kmeans = KMeans(n_clusters=i, init='k-means++', max_iter=300, n_init=10, random_state=0)
    kmeans.fit(X)
    wcss.append(kmeans.inertia_)

# 繪製手肘法圖
plt.plot(range(1, 11), wcss)
plt.scatter(range(1, 11), wcss, color="red")
plt.title('Elbow Method(by kiwi_tech)')
plt.xlabel('Number of clusters')
plt.ylabel('WCSS')  # WCSS代表群內平方和
plt.show()


# 使用輪廓分析法找到最佳的集群數
silhouette_scores = []
for i in range(2, 11):
    kmeans = KMeans(n_clusters=i, init='k-means++', max_iter=300, n_init=10, random_state=0)
    kmeans.fit(X)
    silhouette_scores.append(silhouette_score(X, kmeans.labels_))

# 繪製輪廓分析法圖
plt.plot(range(2, 11), silhouette_scores)
plt.title('Silhouette Analysis(by kiwi_tech)')
plt.xlabel('Number of clusters')
plt.ylabel('Silhouette Score')
plt.show()

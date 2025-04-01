import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('bloom_output.csv')
plt.plot(df['k'], df['false_positive'], '--ob', alpha=0.7)

plt.xlabel('k')
plt.ylabel('false positive %')
plt.title('Bloom Filter False Positive Rate')
plt.grid(True)
plt.show()

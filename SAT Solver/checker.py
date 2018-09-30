import sys
import os
def is_subgraph(G,tempG,s):
	n = len(s)
	for i in range(n):
		for j in range(i+1,n):
			if not ((s[j] in G[s[i]]) or (s[i] in G[s[j]])):
				return False
			try:
				tempG[s[i]].remove(s[j])
			except:
				try:
					tempG[s[j]].remove(s[i])
				except:
					pass
	return True
	
def is_subset(s1,s2):
	#is s2 a subset of s1
	if(len(s2)>len(s1)):
		return False
	else:
		for v in s2:
			if not(v in s1):
				return False
	return True

	
if not(os.path.isfile(sys.argv[1])):
	print("ERROR: Input Graph File does not exist")
	sys.exit(1)
if not(os.path.isfile(sys.argv[2])):
	print("ERROR: Output Subgraph File does not exist")
	sys.exit(1)

fp = open(sys.argv[2],"r")
lines = fp.readlines()
num_lines = len(lines)
fp.close()

if num_lines == 1:
	if lines[0].startswith('0'):
		print('Format is OK')
	else:
		print('ERROR: Wrong Format')
	sys.exit(1)

G={}
tempG={}
with open(sys.argv[1],'r') as f:
	rows = f.readlines()
	i = 0
	for row in rows:
		x = row.strip().split(' ')
		x = [int(v) for v in x]
		if i == 0:
			N = x[0]
			E = x[1]
			K = x[2]
		else:
			if x[0] in G:
				G[x[0]].append(x[1])
				tempG[x[0]].append(x[1])
			else:
				G[x[0]] = [x[1]]
				tempG[x[0]] = [x[1]]
		i+=1
		
for i in range(1,N+1):
	if i not in G:
		G[i] = []
		tempG[i] = []
i=0  
subgraph = {}  
for line in lines:
    if line.startswith('#'):
        n = int(line.split(' ')[1].strip())
        continue
    else:
    	i += 1
    	v = line.split(' ')
    	if (n!=len(v)):
    		print('ERROR(Subgraph %d): Vertices not consistent'%i)
    		sys.exit(1)
    	else:
    		subgraph[i] = [int(x) for x in v]
    		if not (is_subgraph(G,tempG,subgraph[i])):
    			print('ERROR: Subgraph %d is not complete'%i)
    			sys.exit(1)
if i!=K:
	print('ERROR: Output should have %d subgraphs' %K)
	sys.exit(1)
	
for i in range(1,N+1):
	if len(tempG[i])>0:
		print('ERROR: Edge %d-%d is not present in any of the subgraphs'%(i,tempG[i][0]))
		sys.exit(1)

for i in range(1,K+1):
	for j in range(1,K+1):
		if(i!=j):
			if is_subset(subgraph[i],subgraph[j]):
				print('ERROR: subgraph-%d is a subset of subgraph-%d' %(j,i))
				sys.exit(1)
		
print('Output is OK')

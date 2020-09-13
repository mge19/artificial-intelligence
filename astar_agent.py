import time
import random
from copy import deepcopy
from agent import Agent
#Mehmet Gencay Ertürk - 150130118

#  use whichever data structure you like, or create a custom one
import queue
import heapq
from collections import deque


"""
  you may use the following Node class
  modify it if needed, or create your own
"""
class Node():
    
    def __init__(self, parent_node, level_matrix, player_row, player_column, depth, chosen_dir,reverse,h):
        self.parent_node = parent_node
        self.level_matrix = level_matrix
        self.player_row = player_row
        self.player_column = player_column
        self.depth = depth
        self.chosen_dir = chosen_dir
        self.reverse=reverse
        self.h=h
        self.seq = ""
        if (self.chosen_dir == "X"):
            pass
        else:
            self.seq = parent_node.seq + self.chosen_dir
        self.reversedseq=""
        if (self.reverse == "X"):
            pass
        else:
            self.reversedseq = self.reverse + self.parent_node.reversedseq                    
class PriorityQueue: 
    def __init__(self): 
        self.queue = [] 
    def put(self, data):
        min=0
        for i in range(len(self.queue)):
            if self.queue[i].depth+self.queue[i].h < data.depth+data.h: #If f values are equal,<:Expand Last added node. <=:Expand First Added Node
                min=i+1
        self.queue.insert(min,data)
    def get(self):
        item=self.queue[0]
        del self.queue[0] 
        return item
    def size(self):
        return len(self.queue)

class AStarAgent(Agent):
    
    def __init__(self):
        super().__init__()
        self.expanded_node_count = 0
        self.generated_node_count = 1
        self.maximum_node_in_memory_count = 1
    def solve(self, level_matrix, player_row, player_column):
        super().solve(level_matrix, player_row, player_column)
        current_apples=0
        for i in range(len(level_matrix)):
            for j in range(len(level_matrix[0])):
                if level_matrix[i][j]=="A":
                    current_apples+=1
        move_sequence = []
        initial_level_matrix = [list(row) for row in level_matrix] #deepcopy(level_matrix)
        q=PriorityQueue()
        s0 = Node(None, initial_level_matrix, player_row, player_column, 0, "X","X",0)
        visited=[list(row) for row in level_matrix]
        for row in visited:
            for j in range(len(row)):
                row[j]=0
        visited[s0.player_row][s0.player_column]=1
        q.put(s0)
        while current_apples>0:
            n=q.get()
            self.expanded_node_count+=1
            if n.level_matrix[n.player_row][n.player_column-1]!="W" and visited[n.player_row][n.player_column-1]==0:
                n0=Node(n,n.level_matrix,n.player_row,n.player_column-1,n.depth+1,'L','R',abs(n.player_row-player_row)+abs(n.player_column-1-player_column))
                if n.level_matrix[n0.player_row][n0.player_column]=="A":
                    current_apples-=1
                    move_sequence+=n0.seq
                    if current_apples>0:
                        move_sequence+=n0.reversedseq
                n0.level_matrix[n.player_row][n.player_column]="F"
                n0.level_matrix[n.player_row][n.player_column-1]="P"
                visited[n0.player_row][n0.player_column]=1
                q.put(n0)
                self.generated_node_count+=1
            if n.level_matrix[n.player_row][n.player_column+1]!="W" and visited[n.player_row][n.player_column+1]==0:
                n0=Node(n,n.level_matrix,n.player_row,n.player_column+1,n.depth+1,'R','L',abs(n.player_row-player_row)+abs(n.player_column+1-player_column))
                if n.level_matrix[n0.player_row][n0.player_column]=="A":
                    current_apples-=1
                    move_sequence+=n0.seq
                    if current_apples>0:
                        move_sequence+=n0.reversedseq
                n0.level_matrix[n.player_row][n.player_column]="F"
                n0.level_matrix[n.player_row][n.player_column+1]="P"
                visited[n0.player_row][n0.player_column]=1
                q.put(n0)
                self.generated_node_count+=1
            if n.level_matrix[n.player_row-1][n.player_column]!="W" and visited[n.player_row-1][n.player_column]==0:
                n0=Node(n,n.level_matrix,n.player_row-1,n.player_column,n.depth+1,'U','D',abs(n.player_row-1-player_row)+abs(n.player_column-player_column))
                if n.level_matrix[n0.player_row][n0.player_column]=="A":
                    current_apples-=1
                    move_sequence+=n0.seq
                    if current_apples>0:
                        move_sequence+=n0.reversedseq
                n0.level_matrix[n.player_row][n.player_column]="F"
                n0.level_matrix[n.player_row-1][n.player_column]="P"
                visited[n0.player_row][n0.player_column]=1
                q.put(n0)
                self.generated_node_count+=1
            if n.level_matrix[n.player_row+1][n.player_column]!="W"  and visited[n.player_row+1][n.player_column+1]==0:
                n0=Node(n,n.level_matrix,n.player_row+1,n.player_column,n.depth+1,'D','U',abs(n.player_row+1-player_row)+abs(n.player_column-player_column))
                if n.level_matrix[n0.player_row][n0.player_column]=="A":
                    current_apples-=1
                    move_sequence+=n0.seq
                    if current_apples>0:
                        move_sequence+=n0.reversedseq
                n0.level_matrix[n.player_row][n.player_column]="F"
                n0.level_matrix[n.player_row+1][n.player_column]="P"
                visited[n0.player_row][n0.player_column]=1
                q.put(n0)
                self.generated_node_count+=1
            if q.size()>self.maximum_node_in_memory_count:
                self.maximum_node_in_memory_count=q.size()            
        return move_sequence

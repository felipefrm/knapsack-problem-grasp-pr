import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os

ALGORITHMS = 2
INSTANCES = 3
EXECUTIONS = 10
ITERATIONS = 50
graphFolderName = 'graphs'


def readTimeFile(filename):

    f = open(filename, "r")

    exe = []
    instance = []
    algorithm = []

    algorithmCount = 0
    while algorithmCount < ALGORITHMS:
        instancesCount = 0
        while instancesCount < INSTANCES:
            executionsCount = 0
            while executionsCount < EXECUTIONS:
                value = f.readline()
                exe.append(value[:-1])
                executionsCount += 1
            instance.append(exe)
            exe = []
            instancesCount += 1
        algorithm.append(instance)
        instance = []
        algorithmCount += 1

    dataset = []
    for i in range(ALGORITHMS):
        averageInstance = []
        for j in range(INSTANCES):
            sumInstance = 0
            for k in range(EXECUTIONS):
                sumInstance += float(algorithm[i][j][k])
            averageInstance.append(sumInstance/EXECUTIONS)
        dataset.append(averageInstance)

    f.close()
    
    return dataset


def readOutputFile(filename):

    f = open(filename, "r")

    files = []
    executions = []
    iterations = []

    filesCount = 0
    while filesCount < INSTANCES:
        executionsCount = 0
        while executionsCount < EXECUTIONS:
            iterationsCount = 0
            while iterationsCount < ITERATIONS:
                value = f.readline()
                iterations.append(value[:-1])
                iterationsCount += 1
            executions.append(iterations)
            iterations = []
            executionsCount += 1
        files.append(executions)
        executions = []
        filesCount += 1

    dataset = []
    for i in range(INSTANCES):
        averageIteration = []
        for j in range(ITERATIONS):
            sumIter = 0
            for k in range(EXECUTIONS):
                sumIter += float(files[i][k][j])
            averageIteration.append(sumIter/EXECUTIONS)
        dataset.append(averageIteration)

    f.close()

    return dataset


def readIter1000(filename):
    f = open(filename, "r")
    iteration = []
    for i in range(ITERATIONS):
        value = f.readline()
        iteration.append(float(value[:-1]))
    f.close()
    return iteration

def generateGraphs(grasp, grasppr):
    if not os.path.exists(graphFolderName):  
        os.makedirs(graphFolderName)

    for i in range(INSTANCES):
        if i == 0:
            instanceFile = "100 objetos"
        elif i == 1:
            instanceFile = "200 objetos"
        else:
            instanceFile = "500 objetos"

        # print(grasp[i])
        # print(grasppr[i])


        # PLOTA E SALVA OS GRAFICOS
        fig = plt.figure()
        fig.suptitle(instanceFile)
        # plt.yticks(np.arange(0, 10, 0.75))
        plt.plot(grasp[i])
        plt.plot(grasppr[i])
        plt.legend(['GRASP', 'GRASP-PR'], loc='lower right')
        plt.xlabel('Iterações')
        plt.ylabel('FO')
        # plt.show()
        fig.savefig(f'{graphFolderName}/{instanceFile}.png', dpi=fig.dpi)


def generateTimeGraph(time):
    
    time = np.round(time, decimals=3)
    
    labels = ['100 Objetos', '200 Objetos', '500 Objetos']
    grasp = time[0]
    grasppr = time[1]

    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, grasp, width, label='GRASP')
    rects2 = ax.bar(x + width/2, grasppr, width, label='GRASP-PR')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Tempo (s)')
    ax.set_title('Tempo médio por instância')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    def autolabel(rects):
        """Attach a text label above each bar in *rects*, displaying its height."""
        for rect in rects:
            height = rect.get_height()
            ax.annotate('{}'.format(height),
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom')


    autolabel(rects1)
    autolabel(rects2)

    fig.tight_layout()
    
    fig.savefig(f'{graphFolderName}/tempoComputacional.png', dpi=fig.dpi)

def generate1000IterGraph(grasp, grasppr):
        # PLOTA E SALVA OS GRAFICOS
        instanceFile = "1000 Objetos"
        fig = plt.figure()
        fig.suptitle(instanceFile)
        plt.plot(grasp)
        plt.plot(grasppr)
        plt.legend(['GRASP', 'GRASP-PR'], loc='lower right')
        plt.xlabel('Iterações')
        plt.ylabel('FO')
        fig.savefig(f'{graphFolderName}/{instanceFile}.png', dpi=fig.dpi)


grasp = readOutputFile("files/saida_grasp.txt")
grasppr = readOutputFile("files/saida_grasppr.txt")
generateGraphs(grasp, grasppr)

time = readTimeFile("files/tempo.txt")
generateTimeGraph(time)

graspIter1000 = readIter1000("files/1000iter_grasp.txt")
graspprIter1000 = readIter1000("files/1000iter_grasppr.txt")
generate1000IterGraph(graspIter1000, graspprIter1000)
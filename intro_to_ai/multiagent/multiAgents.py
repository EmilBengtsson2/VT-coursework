# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood().asList()
        currentFood = currentGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"

        ghostScore = 0
        for state in newGhostStates:
          pos = state.getPosition()
          score = abs(pos[0]-newPos[0]) + abs(pos[1]-newPos[1])
          if score > ghostScore:
            ghostScore = score

        if ghostScore < 2:
          ghostScore = -2
        else:
          ghostScore = 0

        foodScore = 0
        if currentFood[newPos[0]][newPos[1]]:
          foodScore = 1

        length = max(currentGameState.data.layout.width, currentGameState.data.layout.height)
        if len(newFood) > 0:
          short = 1000000000
        else:
          short = 0
        stateScore = 0
        dist = 0
        for food in newFood:
          dist = abs(food[0]-newPos[0]) + abs(food[1]-newPos[1])
          if dist < short:
            short = dist
        stateScore = -(short / float(length))

        return ghostScore + foodScore + stateScore

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        numAgents = gameState.getNumAgents()
        return self.minimax(gameState, 0, numAgents, numAgents*self.depth)[1]
        util.raiseNotDefined()

    def minimax(self, gameState, agent, numAgents, untilStop):
      actions = gameState.getLegalActions(agent)
      if untilStop == 0 or len(actions) < 1:
        return (self.evaluationFunction(gameState), None)
      untilStop -= 1
      scoreList = []
      for action in actions:
        scoreList.append((self.minimax(gameState.generateSuccessor(agent, action), (agent+1)%numAgents, numAgents, untilStop)[0], action))
      if agent == 0:
        maxi = -1000000000
        maxScore = None
        for score in scoreList:
          if score[0] > maxi:
            maxi = score[0]
            maxScore = score
        return maxScore
      else:
        mini = 1000000000
        minScore = None
        for score in scoreList:
          if score[0] < mini:
            mini = score[0]
            minScore = score
        return minScore


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        value = self.maxValue(gameState, -1000000000, 1000000000, self.depth)[1]
        return value
        util.raiseNotDefined()

    def maxValue(self, gameState, alpha, beta, untilStop):
      v = (-1000000000, None)
      actions = gameState.getLegalActions(0)
      if untilStop == 0 or len(actions) < 1:
        return (self.evaluationFunction(gameState), None)
      for action in actions:
        nextValue = self.minValue(gameState.generateSuccessor(0, action), alpha, beta, gameState.getNumAgents()-2, untilStop)
        if v[0] < nextValue[0]:
          v = (nextValue[0], action)
        if v[0] > beta:
          return v
        alpha = max(alpha, v[0])
      return v

    def minValue(self, gameState, alpha, beta, untilMax, untilStop):
      v = (1000000000, None)
      actions = gameState.getLegalActions(gameState.getNumAgents()-1-untilMax)
      if len(actions) < 1:
        return (self.evaluationFunction(gameState), None)
      for action in actions:
        if untilMax == 0:
          nextValue = self.maxValue(gameState.generateSuccessor(gameState.getNumAgents()-1-untilMax, action), alpha, beta, untilStop-1)
          if v[0] > nextValue[0]:
            v = (nextValue[0], action)
        else:
          nextValue = self.minValue(gameState.generateSuccessor(gameState.getNumAgents()-1-untilMax, action), alpha, beta, untilMax-1, untilStop)
          if v[0] > nextValue[0]:
            v = (nextValue[0], action)
        if v[0] < alpha:
          return v
        beta = min(beta, v[0])
      return v

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        numAgents = gameState.getNumAgents()
        return self.expectimax(gameState, 0, numAgents, numAgents*self.depth)[1]
        util.raiseNotDefined()

    def expectimax(self, gameState, agent, numAgents, untilStop):
      actions = gameState.getLegalActions(agent)
      if untilStop == 0 or len(actions) < 1:
        return (self.evaluationFunction(gameState), None)
      untilStop -= 1
      scoreList = []
      for action in actions:
        scoreList.append((self.expectimax(gameState.generateSuccessor(agent, action), (agent+1)%numAgents, numAgents, untilStop)[0], action))
      if agent == 0:
        maxi = -1000000000
        maxScore = None
        for score in scoreList:
          if score[0] > maxi:
            maxi = score[0]
            maxScore = score
        return maxScore
      else:
        summa = 0
        for score in scoreList:
          summa += score[0]
        return (summa/float(len(scoreList)), None)

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    foods = currentGameState.getFood()
    pacPos = currentGameState.getPacmanPosition()

    minFoodDist = distanceToClosestFood(pacPos, foods, currentGameState.getWalls())
    foodScore = -minFoodDist-(currentGameState.data.layout.width*currentGameState.data.layout.height)*len(foods.asList())
    foodScore = foodScore / float(currentGameState.data.layout.width*currentGameState.data.layout.height)

    ghostScore = 0
    for state in currentGameState.getGhostStates():
      newScore = manhattanDistance(state.getPosition(), pacPos)*state.scaredTimer
      #print newScore
      if newScore > ghostScore:
        ghostScore = newScore
    ghostScore = -0.5*ghostScore

    #print "foodScore: " + str(foodScore) + ", ghostScore: " + str(ghostScore)

    capsuleScore = -2*currentGameState.data.layout.width*currentGameState.data.layout.height*len(currentGameState.getCapsules())
    if ghostScore != 0:
      capsuleScore = 0
      foodScore = 0

    #print "foodScore: " + str(foodScore) + ", ghostScore: " + str(ghostScore)

    return foodScore + currentGameState.getScore() + capsuleScore + ghostScore
    util.raiseNotDefined()

def distanceToClosestFood(pos, foods, walls):
  if len(foods.asList()) == 0:
    return 0
  actionList = [(1, 0), (-1, 0), (0, 1), (0, -1)]
  currentPos = (pos, 0)
  queue = []
  explored = []
  while True:
    if foods[currentPos[0][0]][currentPos[0][1]]:
      return currentPos[1]
    for action in actionList:
      newPos = (currentPos[0][0]+action[0], currentPos[0][1]+action[1])
      if walls[newPos[0]][newPos[1]]:
        continue
      if newPos in explored:
        continue
      explored.append(currentPos[0])
      queue.insert(0, (newPos, currentPos[1]+1))
    currentPos = queue.pop()


# Abbreviation
better = betterEvaluationFunction


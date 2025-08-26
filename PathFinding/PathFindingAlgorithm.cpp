#include "PathFindingAlgorithm.h"
#include "Tile.h"
#include "SetMarker.h"
#include "Game.h"
#include <queue>
#include <set>


std::vector<std::weak_ptr<OpenSetMarker>> AStar::openSetMarkers;
std::vector<std::weak_ptr<CloseSetMarker>> AStar::closeSetMarkers;

std::vector<Tile*> AStar::PathFinding(const std::vector<std::vector<Tile*>>& tiles)
{
	std::vector<std::vector<Node>> nodes;
	std::vector<Tile*> path;

	auto cmp = [](Node* left, Node* right) { return left->f > right->f; };
	std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openNodes(cmp);

	Node* startNode = nullptr;
	Node* endNode = nullptr;
	BuildNode(tiles, nodes, startNode, endNode);
	if (startNode == nullptr || endNode == nullptr)
	{
		printf("There is no start node or end node\n");
		return path;
	}

	Node* currentNode = startNode;
	startNode->g = 0;

	do 
	{
		currentNode->bClose = true;

		for (Node* adjacent : currentNode->mAdjacent)
		{
			if (adjacent->bClose || adjacent->mNodeType == NodeType::Wall)
			{
				continue;
			}

			const float newG = (adjacent->mPos - currentNode->mPos).Length() + currentNode->g;

			if (adjacent->bOpen)
			{
				if (adjacent->g > newG)
				{
					adjacent->mParent = currentNode;
					adjacent->g = newG;
					adjacent->f = adjacent->g + adjacent->h;
					openNodes.emplace(adjacent);
				}
			}
			else
			{
				adjacent->h = Heuristic(*adjacent, *endNode);
				adjacent->g = newG;
				adjacent->f = adjacent->g + adjacent->h;
				adjacent->mParent = currentNode;
				adjacent->bOpen = true;
				openNodes.emplace(adjacent);
			}
		}

		while (!openNodes.empty() && openNodes.top()->bClose)
		{
			openNodes.pop();
		}

		if (openNodes.empty())
		{
			break;
		}

		currentNode = openNodes.top();
		openNodes.pop();

	} while (currentNode != endNode);

	if (currentNode == endNode)
	{
		path.push_back(currentNode->mTile);
		while (currentNode->mParent)
		{
			currentNode = currentNode->mParent;
			path.push_back(currentNode->mTile);
		}
	}

	DrawDebug(nodes);

	return path;
}

AStar::NodeType AStar::ConvertTileStateToNodeType(TileState state)
{
	switch (state)
	{
	case TileState::Default:	return NodeType::Default;
	case TileState::Start:		return NodeType::Start;
	case TileState::End:		return NodeType::End;
	case TileState::Wall:		return NodeType::Wall;
	}

	return NodeType::Default;
}

void AStar::BuildNode(const std::vector<std::vector<Tile*>>& tiles, std::vector<std::vector<Node>>& nodes, Node*& startNode, Node*& endNode)
{
	nodes.resize(tiles.size());
	for (size_t x = 0; x < nodes.size(); ++x)
	{
		nodes[x].resize(tiles[x].size());
	}

	for (size_t x = 0; x < nodes.size(); ++x)
	{
		for (size_t y = 0; y < nodes[x].size(); ++y)
		{
			Node& currentNode = nodes[x][y];
			Tile* currentTile = tiles[x][y];

			currentNode.mNodeIdDebug = Vector2(static_cast<float>(x), static_cast<float>(y));
			currentNode.mTile = currentTile;
			currentNode.mNodeType = ConvertTileStateToNodeType(currentTile->GetTileState());
			currentNode.mPos = currentTile->GetPosition();

			if (currentNode.mNodeType == NodeType::Start)
			{
				startNode = &currentNode;
			}
			if (currentNode.mNodeType == NodeType::End)
			{
				endNode = &currentNode;
			}
		}
	}

	for (size_t x = 0; x < nodes.size(); ++x)
	{
		for (size_t y = 0; y < nodes[x].size(); ++y)
		{
			static const int dx[8] = { -1, 1, 0, 0, -1, -1,  1, 1 };
			static const int dy[8] = { 0, 0,-1, 1, -1,  1, -1, 1 };

			for (int dir = 0; dir < 8; ++dir) 
			{
				const int nx = x + dx[dir];
				const int ny = y + dy[dir];

				if (nx < 0 || nx >= static_cast<int>(nodes.size()))
				{
					continue;
				}
				if (ny < 0 || ny >= static_cast<int>(nodes[x].size()))
				{
					continue;
				}

				if (dir >= 4) {
					const int adjX = nx;
					const int adjY = y;
					const int adjX2 = x;
					const int adjY2 = ny;
					if (nodes[adjX][adjY].mNodeType == NodeType::Wall && nodes[adjX2][adjY2].mNodeType == NodeType::Wall) 
					{
						continue;
					}
				}

				nodes[x][y].mAdjacent.push_back(&nodes[nx][ny]);
			}
		}
	}
}

float AStar::Heuristic(const Node& A, const Node& B)
{
	const float manhattanDist = Math::Abs(A.mPos.x - B.mPos.x) + Math::Abs(A.mPos.y - B.mPos.y);
	return manhattanDist;
}

void AStar::DrawDebug(const std::vector<std::vector<Node>>& nodes)
{
	for (std::weak_ptr<CloseSetMarker> closeSetMarker : closeSetMarkers)
	{
		if (closeSetMarker.lock())
		{
			closeSetMarker.lock()->SetState(Actor::EDead);
		}
	}
	closeSetMarkers.resize(0);

	for (std::weak_ptr<OpenSetMarker> openSetMarker : openSetMarkers)
	{
		if (openSetMarker.lock())
		{
			openSetMarker.lock()->SetState(Actor::EDead);
		}
	}
	openSetMarkers.resize(0);


	for (size_t x = 0; x < nodes.size(); ++x)
	{
		for (size_t y = 0; y < nodes[x].size(); ++y)
		{
			const Node& node = nodes[x][y];

			if (node.bClose)
			{
				closeSetMarkers.push_back(Game::Get()->CreateActor<CloseSetMarker>());
				closeSetMarkers.back().lock()->SetPosition(node.mPos);
			}
			else if (node.bOpen)
			{
				openSetMarkers.push_back(Game::Get()->CreateActor<OpenSetMarker>());
				openSetMarkers.back().lock()->SetPosition(node.mPos);
			}
		}
	}
}

#include "PathFindingAlgorithm.h"
#include "Tile.h"
#include "SetMarker.h"
#include "Game.h"
#include <queue>
#include <set>

template<typename Marker, typename Node>
void SetMarker(size_t index, const Node& node, std::vector<std::weak_ptr<Marker>>& markers)
{
	std::shared_ptr<Marker> marker;

	if (markers.size() > index)
	{
		marker = markers[index].lock();
	}
	else
	{
		markers.push_back(Game::Get()->CreateActor<Marker>());
		marker = markers.back().lock();
	}
	marker->SetPosition(node.mTile->GetPosition());
	marker->SetVisible(true);
}

static std::vector<std::weak_ptr<CloseSetMarker>> closeMarkers;
static std::vector<std::weak_ptr<OpenSetMarker>> openMarkers;

template<typename T>
void DrawDebug(const std::vector<std::vector<T>>& nodes)
{

	size_t closeCnt = 0;
	size_t openCnt = 0;

	for (size_t x = 0; x < nodes.size(); ++x)
	{
		for (size_t y = 0; y < nodes[x].size(); ++y)
		{
			const T& node = nodes[x][y];

			if (node.bClose)
			{
				SetMarker<CloseSetMarker>(closeCnt, node, closeMarkers);
				++closeCnt;
			}
			else if (node.bOpen)
			{
				SetMarker<OpenSetMarker>(openCnt, node, openMarkers);
				++openCnt;
			}
		}
	}

	for (size_t i = closeCnt; i < closeMarkers.size(); ++i)
	{
		closeMarkers[i].lock()->SetVisible(false);
	}

	for (size_t i = openCnt; i < openMarkers.size(); ++i)
	{
		openMarkers[i].lock()->SetVisible(false);
	}
}

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

	if (startNode == endNode)
	{
		path.push_back(startNode->mTile);
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

			currentNode.mNodeIdDebug = Coord(static_cast<int>(x), static_cast<int>(y));
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

float AStar::Heuristic(const Node& a, const Node& b)
{
	const float manhattanDist = Math::Abs(a.mPos.x - b.mPos.x) + Math::Abs(a.mPos.y - b.mPos.y);
	return manhattanDist;
}

std::vector<Tile*> JPS::PathFinding(const std::vector<std::vector<Tile*>>& tiles)
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

	if (startNode == endNode)
	{
		path.push_back(startNode->mTile);
		return path;
	}

	Node* currentNode = startNode;
	startNode->g = 0;
	openNodes.emplace(currentNode);

	while (!openNodes.empty())
	{
		currentNode = openNodes.top();
		openNodes.pop();

		if (currentNode == endNode)
		{
			break;
		}

		currentNode->bClose = true;

		if (Node* jumpPoint = JumpStraight(nodes, currentNode, Coord::Up, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpStraight(nodes, currentNode, Coord::Right, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpStraight(nodes, currentNode, Coord::Down, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpStraight(nodes, currentNode, Coord::Left, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpDiagonal(nodes, currentNode, Coord::UpRight, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpDiagonal(nodes, currentNode, Coord::DownRight, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpDiagonal(nodes, currentNode, Coord::DownLeft, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		if (Node* jumpPoint = JumpDiagonal(nodes, currentNode, Coord::UpLeft, endNode))
		{
			EmplacePoint(currentNode, jumpPoint, endNode, openNodes);
		}

		while (!openNodes.empty() && openNodes.top()->bClose)
		{
			openNodes.pop();
		}
	}

	if (currentNode == endNode)
	{
		path.push_back(currentNode->mTile);
		while (currentNode->mParent)
		{
			const Coord diff = currentNode->mParent->mPos - currentNode->mPos;
			const Coord pathDir = Coord(Math::Sign(diff.x), Math::Sign(diff.y));

			Node* pathNode = currentNode;
			while(pathNode != currentNode->mParent)
			{
				path.push_back(pathNode->mTile);
				pathNode = GetNextNode(nodes, pathNode, pathDir);
				if (pathNode == nullptr)
				{
					break;
				}
			} 

			currentNode = currentNode->mParent;
		}
	}

	DrawDebug(nodes);

	return path;
}

JPS::NodeType JPS::ConvertTileStateToNodeType(TileState state)
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

void JPS::BuildNode(const std::vector<std::vector<Tile*>>& tiles, std::vector<std::vector<Node>>& nodes, Node*& startNode, Node*& endNode)
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

			currentNode.mTile = currentTile;
			currentNode.mNodeType = ConvertTileStateToNodeType(currentTile->GetTileState());
			currentNode.mPos = Coord(static_cast<int>(x), static_cast<int>(y));

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
}

JPS::Node* JPS::JumpStraight(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end)
{
	Node* current = GetNextNode(nodes, start, dir);
	return SearchForcedNeighbor(nodes, current, dir, end);
}

JPS::Node* JPS::JumpDiagonal(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end)
{
	Node* current = GetNextNode(nodes, start, dir);

	while (current)
	{
		if (current == end)
		{
			return current;
		}

		if (SearchForcedNeighbor(nodes, current, Coord(dir.x, 0), end))
		{
			return current;
		}

		if (SearchForcedNeighbor(nodes, current, Coord(0, dir.y), end))
		{
			return current;
		}
		current = GetNextNode(nodes, current, dir);
	}

	return nullptr;
}

JPS::Node* JPS::SearchForcedNeighbor(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end)
{
	Node* current = start;
	Node* nextNode = GetNextNode(nodes, current, dir);

	if (current == end || nextNode == end)
	{
		return end;
	}

	while (current && nextNode)
	{
		if (current->bOpen || current->bClose)
		{
			break;
		}

		Node* currentSide = GetNextNode(nodes, current, Coord(dir.y, dir.x));
		Node* nextSide = GetNextNode(nodes, nextNode, Coord(dir.y, dir.x));
		if (currentSide == nullptr && nextSide)
		{
			return current;
		}

		currentSide = GetNextNode(nodes, current, Coord(dir.y, dir.x) * -1);
		nextSide = GetNextNode(nodes, nextNode, Coord(dir.y, dir.x) * -1);
		if (currentSide == nullptr && nextSide)
		{
			return current;
		}

		current = nextNode;
		nextNode = GetNextNode(nodes, current, dir);

		if (nextNode == end)
		{
			return nextNode;
		}
	}

	return nullptr;
}

int JPS::Heuristic(const Node* a, const Node* b)
{
	const int manhattanDist = Math::Abs(a->mPos.x - b->mPos.x) + Math::Abs(a->mPos.y - b->mPos.y);
	return manhattanDist;
}

template<typename Queue>
void JPS::EmplacePoint(Node* current, Node* emplacePoint, Node* end, Queue& openNodes)
{
	if (emplacePoint->bClose)
	{
		return;
	}
	const float newG = (emplacePoint->mPos - current->mPos).Length() + current->g;

	if (emplacePoint->bOpen)
	{
		if (emplacePoint->g > newG)
		{
			emplacePoint->mParent = current;
			emplacePoint->g = newG;
			emplacePoint->f = emplacePoint->g + emplacePoint->h;
			openNodes.emplace(emplacePoint);
		}
	}
	else
	{
		emplacePoint->h = Heuristic(emplacePoint, end);
		emplacePoint->g = newG;
		emplacePoint->f = emplacePoint->g + emplacePoint->h;
		emplacePoint->mParent = current;
		emplacePoint->bOpen = true;
		openNodes.emplace(emplacePoint);
	}
}

JPS::Node* JPS::GetNextNode(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir)
{
	if (start == nullptr)
	{
		return nullptr;
	}
	const Coord curPos = start->mPos;
	const Coord nextPos = curPos + dir;
	if (static_cast<int>(nodes.size()) <= nextPos.x || nextPos.x < 0)
	{
		return nullptr;
	}

	if (static_cast<int>(nodes[nextPos.x].size()) <= nextPos.y || nextPos.y < 0)
	{
		return nullptr;
	}

	if (nodes[nextPos.x][nextPos.y].mNodeType == NodeType::Wall)
	{
		return nullptr;
	}

	if (nodes[nextPos.x][curPos.y].mNodeType == NodeType::Wall && nodes[curPos.x][nextPos.y].mNodeType == NodeType::Wall)
	{
		return nullptr;
	}

	return &nodes[nextPos.x][nextPos.y];
}
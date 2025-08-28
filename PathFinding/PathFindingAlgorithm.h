#pragma once
#include <vector>
#include <memory>
#include "Math.h"

class Tile;
enum class TileState;
class OpenSetMarker;
class CloseSetMarker;

class AStar
{
public:

	static std::vector<Tile*> PathFinding(const std::vector<std::vector<Tile*>>& tiles);

	enum class NodeType
	{
		Default,
		Start,
		End,
		Wall
	};

	struct Node
	{
		Coord mNodeIdDebug{ 0,0 };
		std::vector<Node*> mAdjacent;
		Node* mParent = nullptr;
		Tile* mTile = nullptr;
		NodeType mNodeType = NodeType::Default;
		Vector2 mPos{ 0,0 };
		float f = 0.0f;
		float g = 0.0f;
		float h = 0.0f;
		bool bClose = false;
		bool bOpen = false;
		bool mBlocked = false;
	};

private:

	static NodeType ConvertTileStateToNodeType(TileState state);
	static void BuildNode(const std::vector<std::vector<Tile*>>& tiles, std::vector<std::vector<Node>>& nodes, Node*& startNode, Node*& endNode);
	static float Heuristic(const Node& a, const Node& b);
};

class JPS
{
public:

	static std::vector<Tile*> PathFinding(const std::vector<std::vector<Tile*>>& tiles);

	enum class NodeType
	{
		Default,
		Start,
		End,
		Wall
	};

	struct Node
	{
		Coord mPos{ 0,0 };
		Node* mParent = nullptr;
		Tile* mTile = nullptr;
		NodeType mNodeType = NodeType::Default;
		float f = 0.0f;
		float g = 0.0f;
		int h = 0;
		bool bClose = false;
		bool bOpen = false;
	};

private:
	static NodeType ConvertTileStateToNodeType(TileState state);
	static void BuildNode(const std::vector<std::vector<Tile*>>& tiles, std::vector<std::vector<Node>>& nodes, Node*& startNode, Node*& endNode);
	
	static Node* JumpStraight(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end);
	static Node* JumpDiagonal(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end);
	static Node* SearchForcedNeighbor(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir, Node* end);
	static int Heuristic(const Node* a, const Node* b);
	template<typename Queue>
	static void EmplacePoint(Node* current, Node* emplacePoint, Node* end, Queue& openNodes);
	static Node* GetNextNode(std::vector<std::vector<Node>>& nodes, Node* start, Coord dir);

	static std::vector<std::weak_ptr<OpenSetMarker>> openSetMarkers;
	static std::vector<std::weak_ptr<CloseSetMarker>> closeSetMarkers;
};


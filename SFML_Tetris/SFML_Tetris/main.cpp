#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

class Figure {
private:
	sf::Vector2i content[4][4];
	int rotationState = 0;
public:
	sf::Vector2i position;
	sf::Vector2i state[4];
	sf::Color color;
	bool exist = false;
	Figure() {}
	Figure(std::vector<std::vector<sf::Vector2i>> _idxs, sf::Color _color) {
		exist = true;
		for (int i = 0; i < 4; i++) {
			state[i] = _idxs[0][i];
			for (int j = 0; j < 4; j++)
				content[i][j] = _idxs[i][j];
		}
		color = _color;
	}
	Figure(const Figure& pattern) {
		exist = true;
		for (int i = 0; i < 4; i++) {
			state[i] = pattern.content[0][i];
			for (int j = 0; j < 4; j++)
				content[i][j] = pattern.content[i][j];
		}
		color = pattern.color;
		position = { 0, 0 };
	}
	void setForGame() {
		position = { 4, -1 };
	}
	void setForPresentation() {
		position = { 1, 1 };
	}
	void setInitialState() {
		rotationState = 0;
		for (int i = 0; i < 4; i++) state[i] = content[rotationState][i];
	}
	void rotateRight() {
		rotationState = (rotationState + 1) % 4;
		for (int i = 0; i < 4; i++) state[i] = content[rotationState][i];
	}
	void rotateLeft() {
		rotationState = (rotationState + 3) % 4;
		for (int i = 0; i < 4; i++) state[i] = content[rotationState][i];
	}
};

Figure HERO({ 
	{ {0, 0}, {1, 0}, {2, 0}, {3, 0} },
	{ {1, 0}, {1, 1}, {1, 2}, {1, 3} },
	{ {0, 0}, {1, 0}, {2, 0}, {3, 0} },
	{ {1, 0}, {1, 1}, {1, 2}, {1, 3} }}, sf::Color(100, 255, 255));
Figure TEEWEE({
	{ {0, 1}, {1, 0}, {1, 1}, {2, 1} },
	{ {1, 0}, {1, 1}, {1, 2}, {2, 1} },
	{ {0, 1}, {1, 1}, {1, 2}, {2, 1} },
	{ {0, 1}, {1, 0}, {1, 1}, {1, 2} } }, sf::Color::Magenta);
Figure SMASHBOY({
	{ {0, 0}, {0, 1}, {1, 0}, {1, 1} },
	{ {0, 0}, {0, 1}, {1, 0}, {1, 1} },
	{ {0, 0}, {0, 1}, {1, 0}, {1, 1} },
	{ {0, 0}, {0, 1}, {1, 0}, {1, 1} } }, sf::Color::Yellow);
Figure ORANGE_RICKY(	{
	{ {0, 1}, {1, 1}, {2, 0}, {2, 1} },
	{ {1, 0}, {1, 1}, {1, 2}, {2, 2} },
	{ {0, 1}, {0, 2}, {1, 1}, {2, 1} },
	{ {0, 0}, {1, 0}, {1, 1}, {1, 2} } }, sf::Color(255, 127, 0));
Figure BLUE_RICKY({
	{ {0, 0}, {0, 1}, {1, 1}, {2, 1} },
	{ {1, 0}, {1, 1}, {1, 2}, {2, 0} },
	{ {0, 1}, {1, 1}, {2, 1}, {2, 2} },
	{ {0, 2}, {1, 0}, {1, 1}, {1, 2} } }, sf::Color::Blue);
Figure CLEVELAND_Z({
	{ {0, 0}, {1, 0}, {1, 1}, {2, 1} },
	{ {0, 1}, {0, 2}, {1, 0}, {1, 1} },
	{ {0, 0}, {1, 0}, {1, 1}, {2, 1} },
	{ {0, 1}, {0, 2}, {1, 0}, {1, 1} } }, sf::Color::Red);
Figure RHODE_ISLAND_Z({
	{ {0, 1}, {1, 0}, {1, 1}, {2, 0} },
	{ {0, 0}, {0, 1}, {1, 1}, {1, 2} },
	{ {0, 1}, {1, 0}, {1, 1}, {2, 0} },
	{ {0, 0}, {0, 1}, {1, 1}, {1, 2} } }, sf::Color::Green);
Figure FIGURES[] = { HERO, TEEWEE, SMASHBOY, ORANGE_RICKY, BLUE_RICKY, CLEVELAND_Z, RHODE_ISLAND_Z };

class Brick {
private:
	sf::RectangleShape shape;
public:
	bool filled = false;
	Brick(){}
	Brick(sf::Vector2f position, sf::Vector2f size, sf::Color color) {
		shape.setFillColor(color);
		shape.setPosition(position);
		shape.setSize(size);
	}
	void setLike(Figure& fig) {
		shape.setFillColor(fig.color);
	}
	void setShadowLike(Figure& fig, sf::Color bgColor) {
		shape.setOutlineThickness(1);
		shape.setOutlineColor(fig.color);
		sf::Color col = bgColor + sf::Color(fig.color.r / 10, fig.color.g / 10, fig.color.b / 10);
		shape.setFillColor(col);
	}
	void setColor(sf::Color color) {
		shape.setFillColor(color);
	}
	sf::Color getColor() {
		return shape.getFillColor();
	}
	void swap(Brick& other) {
		sf::Color tmp = getColor();
		setColor(other.getColor());
		other.setColor(tmp);
		std::swap(filled, other.filled);
	}
	void clear(sf::Color col) {
		filled = false;
		shape.setFillColor(col);
	}
	void clearShadow(sf::Color bgColor) {
		shape.setOutlineThickness(0);
		shape.setFillColor(bgColor);
	}
	void update(sf::RenderWindow& window) {
		window.draw(shape);
	}
};

class Field {
private:
	sf::Vector2f position, size;
	sf::Vector2f cellSize;
	sf::RectangleShape boundingBox;
	std::vector<std::vector<Brick>> matrix;
	sf::Color backgroundColor = sf::Color(7, 7, 7);
	sf::Color defaultColor = sf::Color(25, 25, 25);
	float padding = 3;
	int columnsCount, rowsCount;
	bool isGamingField = false;
	void clearLine(int idx) {
		for (int i = 0; i < columnsCount; i++)
			matrix[idx][i].clear(defaultColor);
	}
	void lowerLine(int idx) {
		for (int i = 0; i < columnsCount; i++)
			matrix[idx][i].swap(matrix[idx + 1][i]);
	}
public:
	Field(float x, float y, float width, float height, int columnsCount, int rowsCount, bool gamingField = false) {
		isGamingField = gamingField;
		position = { x, y };
		size = { width, height };
		cellSize = { width / columnsCount - padding * 2, height / rowsCount - padding * 2 };
		this->columnsCount = columnsCount;
		this->rowsCount = rowsCount;

		boundingBox.setSize(size);
		boundingBox.setPosition(position);
		boundingBox.setFillColor(backgroundColor);

		matrix.resize(rowsCount, std::vector<Brick>(columnsCount));
		for (int i = 0; i < rowsCount; i++) {
			for (int j = 0; j < columnsCount; j++) {
				matrix[i][j] = Brick(
					position + sf::Vector2f((cellSize.x + padding * 2) * j + padding, (cellSize.y + padding * 2) * i + padding),
					cellSize, defaultColor);
			}
		}
	}
	void draw(Figure& figure) {
		if (isGamingField) {
			int _y = figure.position.y;
			do{
				figure.position.y++;
			} while (free(figure));
			figure.position.y--;
			for (int i = 0; i < 4; i++) {
				auto pos = figure.position + figure.state[i];
				if (pos.y >= 0) matrix[pos.y][pos.x].setShadowLike(figure, defaultColor);
			}
			figure.position.y = _y;
		}
		for (int i = 0; i < 4; i++) {
			auto pos = figure.position + figure.state[i];
			if(pos.y >= 0) matrix[pos.y][pos.x].setLike(figure);
		}
	}
	void clear(Figure& figure) {
		for (int i = 0; i < 4; i++) {
			auto pos = figure.position + figure.state[i];
			if(pos.y >= 0) matrix[pos.y][pos.x].clear(defaultColor);
		}
		if (isGamingField) {
			int _y = figure.position.y;
			do {
				figure.position.y++;
			} while (free(figure));
			figure.position.y--;
			for (int i = 0; i < 4; i++) {
				auto pos = figure.position + figure.state[i];
				if (pos.y >= 0) matrix[pos.y][pos.x].clearShadow(defaultColor);
			}
			figure.position.y = _y;
		}
	}
	bool free(Figure& figure) {
		for (int i = 0; i < 4; i++) {
			int _x = figure.state[i].x + figure.position.x, _y = figure.state[i].y + figure.position.y;
			if (_x < 0 || columnsCount <= _x || 
				_y < 0 || rowsCount <= _y ||
				matrix[_y][_x].filled) {
				if(_y >= 0) return false;
			}
		}
		return true;
	}
	bool move(Figure& figure, int x, int y) {
		clear(figure);
		figure.position.x += x, figure.position.y += y;
		if (free(figure)) {
			draw(figure);
			return true;
		}
		figure.position.x -= x, figure.position.y -= y;
		draw(figure);
		return false;
	}
	bool rotate(Figure& figure) {
		clear(figure);
		figure.rotateRight();
		if (free(figure)) {
			draw(figure);
			return true;
		}
		figure.position.x += 1;
		if (free(figure)) {
			draw(figure);
			return true;
		}
		figure.position.x -= 2;
		if (free(figure)) {
			draw(figure);
			return true;
		}
		figure.position.x += 1;
		figure.rotateLeft();
		draw(figure);
		return false;
	}
	void freeze(Figure& figure) {
		for (int i = 0; i < 4; i++) {
			int _x = figure.state[i].x + figure.position.x, _y = figure.state[i].y + figure.position.y;
			if (_y >= 0) {
				matrix[_y][_x].filled = true;
				matrix[_y][_x].clearShadow(figure.color);
			}
		}
	}
	int checkLines() {
		int clearedCnt = 0;
		for (int i = rowsCount - 1; i >= 0; i--) {
			bool filled = true;
			for (int j = 0; j < columnsCount; j++) {
				if (!matrix[i][j].filled) {
					filled = false;
					break;
				}
			}
			if (!filled) continue;
			clearedCnt++;

			clearLine(i);
			for (int k = i - 1; k >= 0; k--)
				lowerLine(k);

			i++;
		}
		return clearedCnt;
	}
	void update(sf::RenderWindow& window) {
		window.draw(boundingBox);
		for (size_t i = 0; i < rowsCount; i++) {
			for (size_t j = 0; j < columnsCount; j++) {
				matrix[i][j].update(window);
			}
		}
	}
};

Figure generateFigure() {
	return Figure(FIGURES[rand() % 7]);
}

int putOnTheBottom(Field& field, Field& nextFigureField, Figure& curFigure, Figure& nextFigure, int& score) {
	while (field.move(curFigure, 0, 1));
	field.freeze(curFigure);

	int ret = field.checkLines();
	score += 10 * ret * ret;

	nextFigureField.clear(nextFigure);
	
	curFigure = nextFigure;
	nextFigure = generateFigure();
	
	curFigure.setForGame();
	nextFigure.setForPresentation();

	field.draw(curFigure);
	nextFigureField.draw(nextFigure);
	
	return ret;
}

void putInHoldField(Field& field, Field& nextField, Field& holdField, Figure& curFigure, Figure& nextFigure, Figure& holdFigure) {
	if (holdFigure.exist) {
		field.clear(curFigure);
		holdField.clear(holdFigure);
		std::swap(curFigure, holdFigure);

		curFigure.setForGame();
		field.draw(curFigure);
		holdFigure.setForPresentation();
		holdField.draw(holdFigure);
	}
	else {
		field.clear(curFigure);
		nextField.clear(nextFigure);
		
		holdFigure = curFigure;
		curFigure = nextFigure;
		nextFigure = generateFigure();
		
		holdFigure.setInitialState();

		nextFigure.setForPresentation();
		holdFigure.setForPresentation();
		curFigure.setForGame();

		field.draw(curFigure);
		nextField.draw(nextFigure);
		holdField.draw(holdFigure);
	}
}

void gameOver(sf::RenderWindow& window) {
	window.close();
}

void writeToFile(std::string name, int& var) {
	std::ofstream fout("record.txt");
	fout << var;
}

int main() {
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(675, 800), "Tetris");
	sf::Clock timer;

	window.setFramerateLimit(30);

	Field field(50, 50, 350, 700, 10, 20, true);
	Field nextFigureField(450, 50, 175, 140, 5, 4);
	Field holdFigureField(450, 240, 175, 140, 5, 4);

	sf::Font font;
	font.loadFromFile("Whipsmart.ttf");

	int score = 0;
	int bestScore = 0;
	int level = 1, _prevLevel = 1;
	int linesCleared = 0, _prevLinesCleared = 0;
	int fallDelay = 600;

	std::ifstream fin("record.txt");
	fin >> bestScore;
 
	sf::Text scoreLabel("score:", font, 50);
	sf::Text bestScoreLabel("the best:", font, 50);
	sf::Text levelLabel("level:", font, 50);

	sf::Text scoreText("0", font, 50);
	sf::Text bestScoreText(std::to_string(bestScore), font, 50);
	sf::Text levelText("1", font, 50);

	scoreLabel.setPosition(450, 400);
	scoreText.setPosition(500, 450);
	bestScoreLabel.setPosition(450, 510);
	bestScoreText.setPosition(500, 560);
	levelLabel.setPosition(450, 620);
	levelText.setPosition(500, 670);


	Figure curFigure = generateFigure();
	Figure nextFigure = generateFigure();
	Figure holdFigure;

	curFigure.setForGame();
	nextFigure.setForPresentation();
	field.draw(curFigure);
	nextFigureField.draw(nextFigure);

	bool left = false, right = false, down = false, space = false;
	int firstDelay = 300, secondDelay = 150, downDelay = 100;
	int leftDelay = firstDelay, rightDelay = firstDelay;
	sf::Clock leftTimer, rightTimer, downTimer;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Left && !left) {
					left = true;
					leftDelay = firstDelay;
					field.move(curFigure, -1, 0);
					leftTimer.restart();
				}
				else if (event.key.code == sf::Keyboard::Right && !right) {
					right = true;
					rightDelay = firstDelay;
					field.move(curFigure, 1, 0);
					rightTimer.restart();
				}
				else if (event.key.code == sf::Keyboard::Down && !down) {
					down = true;
					field.move(curFigure, 0, 1);
					downTimer.restart();
				}
				else if (event.key.code == sf::Keyboard::Space && !space) {
					space = true;
					linesCleared += putOnTheBottom(field, nextFigureField, curFigure, nextFigure, score);
					if (!field.free(curFigure))
						gameOver(window);
				}
				else if (event.key.code == sf::Keyboard::Up)
					field.rotate(curFigure);
				else if (event.key.code == sf::Keyboard::LShift)
					putInHoldField(field, nextFigureField, holdFigureField, curFigure, nextFigure, holdFigure);
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Left)
					left = false;
				else if (event.key.code == sf::Keyboard::Right)
					right = false;
				else if (event.key.code == sf::Keyboard::Down)
					down = false;
				else if (event.key.code == sf::Keyboard::Space)
					space = false;
			}
		}
		if (left && leftTimer.getElapsedTime().asMilliseconds() > leftDelay) {
			leftTimer.restart();
			leftDelay = secondDelay;
			field.move(curFigure, -1, 0);
		}
		if (right && rightTimer.getElapsedTime().asMilliseconds() > rightDelay) {
			rightTimer.restart();
			rightDelay = secondDelay;
			field.move(curFigure, 1, 0);
		}
		if (down && downTimer.getElapsedTime().asMilliseconds() > downDelay) {
			downTimer.restart();
			field.move(curFigure, 0, 1);
		}

		window.clear(sf::Color::Black);
		
		if (timer.getElapsedTime().asMilliseconds() > fallDelay) {
			if (!field.move(curFigure, 0, 1)) {
				linesCleared += putOnTheBottom(field, nextFigureField, curFigure, nextFigure, score);
				if (!field.free(curFigure))
					gameOver(window);
			}
			timer.restart();
		}
		if (linesCleared != _prevLinesCleared) {
			_prevLinesCleared = linesCleared;
			scoreText.setString(std::to_string(score));
			if (score > bestScore) {
				bestScore = score;
				writeToFile("record.txt", bestScore);
				bestScoreText.setString(std::to_string(bestScore));
			}
			level = 1 + linesCleared / 10;
			if (_prevLevel != level) {
				fallDelay -= (level - _prevLevel) * 20;
				_prevLevel = level;
				levelText.setString(std::to_string(level));
			}
		}

		field.update(window);
		nextFigureField.update(window);
		holdFigureField.update(window);
		
		window.draw(scoreLabel);
		window.draw(scoreText);
		window.draw(bestScoreLabel);
		window.draw(bestScoreText);
		window.draw(levelLabel);
		window.draw(levelText);

		window.display();
	}

	return 0;
}
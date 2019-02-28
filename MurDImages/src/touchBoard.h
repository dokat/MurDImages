#pragma once
#include <vector>
#include "word.h"
#include "key.h"
#include "animatable.h"

using namespace std;

class touchBoard : public animatable
{
public:
	touchBoard();
	~touchBoard();

	void clear();
	void addAllKey(vector<char> keys);	
	void addAllKey(vector<string> keys);

	void setDimension(int width, int height);
	void draw();
	void refresh();
	bool mousePressed(int x, int y);
	void mouseReleased();
	void sort();
	void sortFilter();

	//indique si le clavier de mots-clés est visible
	inline bool isVisible() { return visible;  }

	//Affiche le touchboard (partie basse)
	void show();

	//Cache le touchboard (partie haute)
	void hide();

	void onTimeAnimationFinished() override;
	void onAlphaAnimationFinished() override;

	static ofEvent<void> backToLettersEvent;
	static ofEvent<void> zoomOutEvent;
	static ofEvent<void> zoomInEvent;
	static ofEvent<void> centerEvent;
	static ofEvent<string> keywordSelectedEvent;
	static ofEvent<string> keywordDeletedEvent;


private:
	void addKey(char charLabel);
	void addKey(string stringLabel, bool sameSize);
	void setCurrentPage(int page);
	void onKeyPressed(key  &key);
	void onZoomInPressed(key &key);
	void onZoomOutPressed(key &key);
	void onCenterPressed(key &key);

	//Constantes
	const int PADDING_TOP = 25;
	const int PADDING_LEFT = 25;

	const string LABEL_RETOUR = "Retour";
	const string LABEL_NEXT = "Mots-clés suivants";
	const string LABEL_SELECT = "Sélectionnez un mot-clé :";
	const string LABEL_DELETE = "Mots-clé sélectionnés (cliquez dessus pour en retirer) :";
	const string LABEL_ZOOMOUT = "-";
	const string LABEL_ZOOMIN = "+";
	const string LABEL_CENTER = ".";

	ofTrueTypeFont myFont;
	vector<key*> keys;
	vector<key*> selectedKeywords;

	//si à true, on est en train d'afficher des lettres, si false, ce sont les mots-clés
	bool isLetterPad = true;

	bool visible = true;

	key * nextKey;
	bool nextVisible;
	key * backKey;
	bool backVisible;

	key * zoomInKey;
	key * zoomOutKey;
	key * centerKey;

	//indique le nombre de pages de mots-clés existantes
	int numPages;
	//indique le numéro de la page courante
	int currentPage;

	//Position en Y du label "Sélectionnez un mot-clé :"
	int lblKeywordsPositionY;
	//Position en Y du label "Mots-clé sélectionnés..."
	int lblSelectedKeywordsPositionY;

	int width, height;

	bool hasToRefresh = true;
};

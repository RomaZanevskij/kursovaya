#include <iostream>
using namespace std;

int main() {
	setlocale(LC_ALL, "RU");

	return 0;
}

class Character {
protected:
  string name;
    string element;
    int damage;
  int health;
  int maxHealth;
    int mana;
    int maxMana;
    bool isAlive;
public:
    Character(string name, string element, int health, int damage, int mana) {
        this->name = name;
        this->element = element;
        this->health = health;
        this->maxHealth = health;
        this->damage = damage;
        this->mana = mana;
        this->maxMana = mana;
        this->isAlive = true;
    }

    virtual ~Character() {}

    void TakeDamage(int amount) {
        health -= amount;
        cout << name << " получил " << amount << " урона" << endl;
        if (health <= 0) {
            health = 0;
            isAlive = false;
            cout << name << " повержен" << endl;
        }
    }

    void Heal(int amount) {
        health += amount;
        if (health >= maxHealth) {
            health = maxHealth;
        }
        cout << name << " восстановил " << amount << " HP (Текущее HP: " << health << ")" << endl;
    }

    virtual void Attack(Character& target) {
        cout << name << " атакует " << target.name << " и наносит " << damage << " урона" << endl;
        target.TakeDamage(damage);
    }

    virtual void ShowStats() {
        cout << name << " | HP: " << health << "/" << maxHealth << " | Урон: " << damage << endl;
    }

    string GetName() const { return name; }
    string GetElement() const{ return element; }
    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }
    bool GetIsAlive() const { return isAlive; }
    int GetDamage() const { return damage; }
    int GetMana() const { return mana; }
};

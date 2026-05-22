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

//создание подкласса маг, новый персонаж который использует свои спсобности 
class Mage : public Character {
private:

    bool shieldActive = false;
    bool poisonActive = false;

public:

    Mage(string name)
        : Character(name, "Fire", 80, 15, 120)
    {}

    // Обычная атака
    void Attack(Character& target) override {

        cout << name
             << " атакует магическим зарядом!\n";

        target.TakeDamage(damage);
    }

    // Fireball
    void Fireball(Character& target) {

        if (mana >= 30) {

            mana -= 30;

            cout << name
                 << " использует Fireball!\n";

            target.TakeDamage(damage + 35);

            cout << "Мана: "
                 << mana
                 << "/"
                 << maxMana
                 << endl;
        }
        else {

            cout << "Недостаточно маны!\n";
        }
    }

    // Отравление
    void Poison(Character& target) {

        if (mana >= 25) {

            mana -= 25;

            poisonActive = true;

            cout << name
                 << " накладывает отравление на "
                 << target.GetName()
                 << endl;

            target.TakeDamage(10);

            cout << target.GetName()
                 << " получает периодический урон!\n";

            cout << "Мана: "
                 << mana
                 << "/"
                 << maxMana
                 << endl;
        }
        else {

            cout << "Недостаточно маны!\n";
        }
    }

    // Магический щит
    void Shield() {

        if (mana >= 20) {

            mana -= 20;

            shieldActive = true;

            cout << name
                 << " использует магический щит!\n";

            cout << "Следующий входящий урон будет уменьшен.\n";

            cout << "Мана: "
                 << mana
                 << "/"
                 << maxMana
                 << endl;
        }
        else {

            cout << "Недостаточно маны!\n";
        }
    }

    // Быстрое лечение
    void FastHeal() {

        if (mana >= 35) {

            mana -= 35;

            int healAmount = 40;

            Heal(healAmount);

            cout << name
                 << " быстро восстанавливает здоровье!\n";

            cout << "Мана: "
                 << mana
                 << "/"
                 << maxMana
                 << endl;
        }
        else {

            cout << "Недостаточно маны!\n";
        }
    }

    // Проверка щита
    void TakeDamageWithShield(int amount) {

        if (shieldActive) {

            amount /= 2;

            cout << "Щит уменьшает урон!\n";

            shieldActive = false;
        }

        TakeDamage(amount);
    }

    // Характеристики
    void ShowStats() override {

        cout << "\n===== MAGE =====\n";

        cout << "Имя: "
             << name
             << endl;

        cout << "Стихия: "
             << element
             << endl;

        cout << "HP: "
             << health
             << "/"
             << maxHealth
             << endl;

        cout << "Mana: "
             << mana
             << "/"
             << maxMana
             << endl;

        cout << "Damage: "
             << damage
             << endl;

        cout << "Shield: "
             << (shieldActive ? "Active" : "Inactive")
             << endl;
    }
};

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;
class Character;
class Mage;
class Warrior;

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

    virtual void TakeDamage(int amount) {
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
    void TakeDamage(int amount) override {

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

class Warrior : public Character {
private:
    int rage;
    int maxRage;
    bool shieldActive;
    int damageBonus;

public:
    Warrior(string name)
        : Character(name, "Earth", 180, 15, 0)  // Нет маны, много здоровья
    {
        rage = 0;
        maxRage = 100;
        shieldActive = false;
        damageBonus = 0;
    }

    // Обычная атака
    void Attack(Character& target) override {
        cout << name << " наносит удар мечом!" << endl;
        
        int totalDamage = damage + damageBonus;
        target.TakeDamage(totalDamage);
        
        rage += 15;
        if (rage > maxRage) rage = maxRage;
        
        cout << "Ярость: " << rage << "/" << maxRage << endl;
    }

    // Тяжелый удар мечом
    void HeavyStrike(Character& target) {
        if (rage >= 40) {
            rage -= 40;
            cout << name << " использует ТЯЖЕЛЫЙ УДАР МЕЧОМ!" << endl;
            target.TakeDamage(damage + 25 + damageBonus);
            cout << "Ярость: " << rage << "/" << maxRage << endl;
        }
        else {
            cout << "Недостаточно ярости! Нужно 40" << endl;
        }
    }

    // Укрытие за щитом
    void Shield() {
        if (rage >= 20) {
            rage -= 20;
            shieldActive = true;
            cout << name << " укрывается за щитом!" << endl;
            cout << "Следующий урон будет снижен на 35%" << endl;
            cout << "Ярость: " << rage << "/" << maxRage << endl;
        }
        else {
            cout << "Недостаточно ярости! Нужно 20" << endl;
        }
    }

    // Получение урона со щитом
    void TakeDamage(int amount) override {
        if (shieldActive) {
            amount = amount * 0.65;
            cout << "Щит снижает урон!" << endl;
            shieldActive = false;
        }
        
        int oldHealth = health;
        TakeDamage(amount);
        
        // Пассивная способность: за каждые 15 потерянного HP +3 к урону
        int healthLost = oldHealth - health;
        if (healthLost > 0) {
            int bonus = (healthLost / 15) * 3;
            damageBonus += bonus;
            if (bonus > 0) {
                cout << "Ярость берсерка! Урон увеличен на " << bonus << endl;
            }
        }
    }

    // Показать характеристики
    void ShowStats() override {
        cout << "\n===== WARRIOR =====" << endl;
        cout << "Имя: " << name << endl;
        cout << "Стихия: " << element << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Ярость: " << rage << "/" << maxRage << endl;
        cout << "Урон: " << damage << " +" << damageBonus << endl;
        cout << "Щит: " << (shieldActive ? "Активен" : "Не активен") << endl;
    }
};

class Ninja : public Character {
private:
    bool invisible = false;    // Невидимость активна
    bool smokeActive = false;   // Дым активен

public:
    Ninja(string name)
        : Character(name, "Wind", 85, 18, 110)  // HP 85, урон 18, макс мана 110
    {}

    // Обычная атака
    void Attack(Character& target) override {
        int bonus = 0;
        
        if (invisible) {
            bonus = 20;
            cout << "(Невидимость увеличивает урон!)\n";
            invisible = false;  // После атаки невидимость пропадает
        }
        
        cout << name << " атакует из теней!\n";
        target.TakeDamage(damage + bonus);
    }

    // Скрытный удар (уходит в невидимость)
    void StealthStrike() {
        if (mana >= 30) {
            mana -= 30;
            invisible = true;
            
            cout << name << " использует Скрытный удар и становится невидимым!\n";
            cout << "Следующая атака нанесёт дополнительный урон.\n";
            cout << "Мана: " << mana << "/" << maxMana << endl;
        }
        else {
            cout << "Недостаточно маны!\n";
        }
    }

    // Дымовая завеса (увеличивает ловкость/урон на время)
    void SmokeScreen() {
        if (mana >= 25) {
            mana -= 25;
            smokeActive = true;
            
            cout << name << " создаёт дымовую завесу!\n";
            cout << "Ловкость увеличена! +15 к урону на 3 хода.\n";
            cout << "Мана: " << mana << "/" << maxMana << endl;
        }
        else {
            cout << "Недостаточно маны!\n";
        }
    }

    // Сюрикены (быстрая атака)
    void Shuriken(Character& target) {
        if (mana >= 20) {
            mana -= 20;
            
            cout << name << " бросает сюрикены!\n";
            target.TakeDamage(damage + 10);
            
            cout << "Мана: " << mana << "/" << maxMana << endl;
        }
        else {
            cout << "Недостаточно маны!\n";
        }
    }

    // Быстрое восстановление
    void FastHeal() {
        if (mana >= 35) {
            mana -= 35;
            
            int healAmount = 40;
            Heal(healAmount);
            
            cout << name << " быстро восстанавливает здоровье!\n";
            cout << "Мана: " << mana << "/" << maxMana << endl;
        }
        else {
            cout << "Недостаточно маны!\n";
        }
    }

    // Получение урона с учётом дыма
    void TakeDamage (int amount) override {
        if (smokeActive) {
            amount = amount / 2;
            cout << "Дымовая завеса уменьшает урон вдвое!\n";
            smokeActive = false;  // Дым рассеивается после одного удара
        }
        
        TakeDamage(amount);
    }

    // Характеристики
    void ShowStats() override {
        cout << "\n===== NINJA =====\n";
        cout << "Имя: " << name << endl;
        cout << "Стихия: " << element << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Мана: " << mana << "/" << maxMana << endl;
        cout << "Урон: " << damage << endl;
        cout << "Невидимость: " << (invisible ? "Активна" : "Не активна") << endl;
        cout << "Дым: " << (smokeActive ? "Активен" : "Не активен") << endl;
    }
};

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

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
    string GetElement() const { return element; }
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
    int poisonTurnsLeft = 0;
    Character* poisonedTarget = nullptr;
public:

    Mage(string name)
        : Character(name, "Fire", 80, 15, 120)
    {
    }

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
            poisonTurnsLeft = 3;
            poisonedTarget = &target;

            cout << name
                << " накладывает отравление на "
                << target.GetName()
                << endl;

            target.TakeDamage(10);

            cout << target.GetName()
                << " получает периодический урон в течение 3 ходов!\n";

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
        Character::TakeDamage(amount);
    }

    // Периодический урон от отравления
    void ApplyPoisonDamage() {
        if (poisonActive && poisonedTarget != nullptr && poisonTurnsLeft > 0) {
            if (poisonedTarget->GetIsAlive()) {
                cout << "Яд " << name << " наносит урон " << poisonedTarget->GetName() << "!\n";
                poisonedTarget->TakeDamage(10);
            }
            poisonTurnsLeft--;
            if (poisonTurnsLeft <= 0) {
                poisonActive = false;
                poisonedTarget = nullptr;
                cout << "Действие отравления закончилось.\n";
            }
        }
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

        cout << "Poison: "
            << (poisonActive ? "Active (" + to_string(poisonTurnsLeft) + " turns left)" : "Inactive")
            << endl;
    }
    void ResetShield() { shieldActive = false; }
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
        Character::TakeDamage(amount);

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
    int GetRage() { return rage; }
    void ResetShield() { shieldActive = false; }
};

class Ninja : public Character {
private:
    bool invisible = false;    // Невидимость активна
    bool smokeActive = false;   // Дым активен
    int smokeTurnsLeft = 0;     // Сколько ходов осталось действовать дыму
    int smokeDamageBonus = 0;   // Бонус к урону от дыма

public:
    Ninja(string name)
        : Character(name, "Wind", 85, 18, 110)  // HP 85, урон 18, макс мана 110
    {
    }

    // Обычная атака
    void Attack(Character& target) override {
        int bonus = 0;

        if (invisible) {
            bonus += 20;
            cout << "(Невидимость увеличивает урон!)\n";
            invisible = false;  // После атаки невидимость пропадает
        }

        if (smokeActive && smokeTurnsLeft > 0) {
            bonus += smokeDamageBonus;
            cout << "(Дымовая завеса увеличивает урон на " << smokeDamageBonus << "!)\n";
        }

        cout << name << " атакует из теней!\n";
        target.TakeDamage(damage + bonus);

        // Уменьшаем счётчик ходов дыма после атаки
        if (smokeActive) {
            smokeTurnsLeft--;
            if (smokeTurnsLeft <= 0) {
                smokeActive = false;
                smokeDamageBonus = 0;
                cout << "Дымовая завеса рассеялась.\n";
            }
        }
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
            smokeTurnsLeft = 3;
            smokeDamageBonus = 15;

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
    void TakeDamage(int amount) override {
        if (smokeActive) {
            amount = amount / 2;
            cout << "Дымовая завеса уменьшает урон вдвое!\n";
            smokeActive = false;
            smokeTurnsLeft = 0;
            smokeDamageBonus = 0;
        }

        Character::TakeDamage(amount);
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
        cout << "Дым: " << (smokeActive ? "Активен (осталось ходов: " + to_string(smokeTurnsLeft) + ", бонус +" + to_string(smokeDamageBonus) + ")" : "Не активен") << endl;
    }
    void ResetInvisible() { invisible = false; }
};

class Skeleton : public Character {
private:
    int turnsWithoutDamage;     // Количество ходов без получения урона
    bool hasBlocked;            // Был ли уже заблокирован первый удар

public:
    Skeleton(string name)
        : Character(name, "Darkness", 120, 15, 0) {
        turnsWithoutDamage = 0;
        hasBlocked = false;
    }

    // Пассивка 1: Регенерация здоровья если нет урона 2 хода
    void RegenerateHealth() {
        if (turnsWithoutDamage >= 2 && health < maxHealth) {
            int healAmount = 20;
            if (turnsWithoutDamage >= 4) {
                healAmount = 40;  // Если нет урона 4+ хода, восстанавливает 40 HP
            }

            int newHealth = health + healAmount;
            if (newHealth > maxHealth) {
                health = maxHealth;
            }
            else {
                health = newHealth;
            }
            cout << name << " не получал урона " << turnsWithoutDamage
                << " хода! Пассивная регенерация: +" << healAmount << " HP!\n";
            cout << "Здоровье: " << health << "/" << maxHealth << "\n";
        }
    }

    // Переопределяем получение урона для отслеживания ходов
    void TakeDamage(int amount) override {
        // Сбрасываем счётчик ходов без урона
        turnsWithoutDamage = 0;
        // Сбрасываем блокировку при получении урона
        hasBlocked = false;

        // Вызываем обычное получение урона
        Character::TakeDamage(amount);
    }

    // Атака с пассивкой "урон только со второго удара"
    void Attack(Character& target) override {
        cout << name << " атакует!\n";

        if (!hasBlocked) {
            cout << "Пассивная способность! Первый удар заблокирован!\n";
            cout << name << " готовится к следующей атаке...\n";
            hasBlocked = true;
            // Не наносим урон при первом ударе
            return;
        }

        // Второй удар наносит урон
        cout << name << " наносит урон со второго удара!\n";
        target.TakeDamage(damage);

        // Сбрасываем флаг после успешной атаки
        hasBlocked = false;
    }

    // Метод для обновления состояния (вызывать каждый ход)
    void EndOfTurn() {
        turnsWithoutDamage++;     // Увеличиваем счётчик ходов без урона
        RegenerateHealth();       // Проверяем регенерацию
    }

    // Переопределяем метод для отображения характеристик
    void ShowStats() override {
        cout << "\n===== SKELETON =====" << endl;
        cout << "Имя: " << name << endl;
        cout << "Стихия: " << element << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Урон: " << damage << endl;
        cout << "Ходов без урона: " << turnsWithoutDamage << endl;

        if (turnsWithoutDamage >= 2) {
            cout << "Регенерация активна!" << endl;
        }
        else {
            cout << "Регенерация не активна" << endl;
        }

        cout << "Статус блока: " << (hasBlocked ? "Первый удар заблокирован" : "Готов наносить урон") << endl;
        cout << "===================\n";
    }

    // Геттер для проверки статуса блока (если нужно)
    bool HasBlocked() const { return hasBlocked; }

    // Сброс пассивки (если нужно вызвать извне)
    void ResetBlockPassive() {
        hasBlocked = false;
        cout << "Пассивка сброшена! Следующая атака снова будет заблокирована.\n";
    }
};

class Zombie : public Character {
private:
    int turnsWithoutDamage;     // Счётчик ходов без получения урона

public:
    Zombie(string name)
        : Character(name, "Darkness", 100, 12, 0) {
        turnsWithoutDamage = 0;
    }

    // Пассивка: Регенерация каждый ход если не получал урон
    void Regeneration() {
        if (turnsWithoutDamage >= 1 && health < maxHealth) {
            int healAmount = 10;

            int newHealth = health + healAmount;
            if (newHealth > maxHealth) {
                health = maxHealth;
            }
            else {
                health = newHealth;
            }
            cout << name << " регенерирует! +" << healAmount << " HP\n";
            cout << "Здоровье: " << health << "/" << maxHealth << "\n";
        }
    }

    void TakeDamage(int amount) override {
        turnsWithoutDamage = 0;
        Character::TakeDamage(amount);
    }

    void Attack(Character& target) override {
        cout << name << " атакует!\n";
        target.TakeDamage(damage);
    }

    void EndOfTurn() {
        turnsWithoutDamage++;
        Regeneration();
    }

    void ShowStats() override {
        cout << "\n===== ZOMBIE =====" << endl;
        cout << "Имя: " << name << endl;
        cout << "Стихия: " << element << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Урон: " << damage << endl;
        cout << "Ходов без урона: " << turnsWithoutDamage << endl;
        cout << "==================\n";
    }
};

class Ghost : public Character {
private:
    bool isEthereal;            // Эфирная форма активна
    int turnsUntilPhaseShift;    // Ходов до следующего фазового сдвига
    int etherealTurnsLeft;       // Сколько ходов осталось в эфирной форме

public:
    Ghost(string name)
        : Character(name, "Wind", 70, 14, 0) {
        isEthereal = false;
        turnsUntilPhaseShift = 3;
        etherealTurnsLeft = 0;
    }

    // Пассивка: Фазовый сдвиг каждые 3 хода
    void PhaseShift() {
        if (turnsUntilPhaseShift <= 0 && !isEthereal) {
            isEthereal = true;
            etherealTurnsLeft = 1;  // Действует 1 ход
            cout << name << " входит в эфирную форму! Уклонение от следующих "
                << etherealTurnsLeft << " атак!\n";
            turnsUntilPhaseShift = 3;  // Сброс счётчика
        }
    }

    void TakeDamage(int amount) override {
        if (isEthereal && etherealTurnsLeft > 0) {
            cout << name << " уклоняется в эфирной форме!\n";
            etherealTurnsLeft--;
            if (etherealTurnsLeft == 0) {
                isEthereal = false;
                cout << name << " выходит из эфирной формы.\n";
            }
            return;
        }
        Character::TakeDamage(amount);
    }

    void Attack(Character& target) override {
        cout << name << " атакует!\n";
        target.TakeDamage(damage);
    }

    void EndOfTurn() {
        if (!isEthereal) {
            turnsUntilPhaseShift--;
            PhaseShift();
        }
        else {
            etherealTurnsLeft--;
            if (etherealTurnsLeft == 0) {
                isEthereal = false;
                cout << name << " выходит из эфирной формы.\n";
            }
        }
    }

    void ShowStats() override {
        cout << "\n===== GHOST =====" << endl;
        cout << "Имя: " << name << endl;
        cout << "Стихия: " << element << endl;
        cout << "HP: " << health << "/" << maxHealth << endl;
        cout << "Урон: " << damage << endl;
        cout << "Эфирная форма: " << (isEthereal ? "Активна" : "Не активна");
        if (isEthereal) {
            cout << " (осталось уклонений: " << etherealTurnsLeft << ")";
        }
        cout << endl;
        cout << "Ходов до фазового сдвига: " << turnsUntilPhaseShift << endl;
        cout << "================\n";
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    srand(time(nullptr));

    return 0;
}

#pragma once
#include "NPC.h"

struct Knight : public NPC
{
    Knight(std::string name, int x, int y);
    Knight(std::istream &is);

    void print() override;
    void print(std::ofstream& os) const override;

    bool fight(std::shared_ptr<Dragon> other) override;
    bool fight(std::shared_ptr<Knight> other) override;
    bool fight(std::shared_ptr<Princess> other) override;
    bool accept(std::shared_ptr<NPC> visitor) override;

    void save(std::ostream &os) override;

    friend std::ostream &operator<<(std::ostream &os, Knight &knight);
};
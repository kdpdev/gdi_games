#pragma once

#include <engine/core/action.h>
#include <engine/core/err.h>

#include <map>

namespace Engine
{
  namespace Core
  {
    template <class KeyType>
    class ActionsRegistry
    {
      using Key = KeyType;
      using Map = std::map<KeyType, Action>;

    public:
      bool HasAction(const Key& key) const
      {
        return Get(key) != 0;
      }

      const Action* Find(const Key& key) const && = delete;
      const Action* Find(const Key& key) const &
      {
        auto iter = m_actions.find(key);
        return iter != m_actions.end() ? &iter->second : nullptr;
      }

      Action Get(const Key& key) const
      {
        ERR_THROW_IF_NOT(HasAction(key), "Action is not found.");
        return *Find(key);
      }

      void Add(const Key& key, Action action)
      {
        ERR_THROW_IF(HasAction(key), "Action is already added.");
        ERR_THROW_IF_NOT(action, "Action is empty.");
        m_actions.emplace(key, std::move(action));
      }

      void Replace(const Key& key, Action action)
      {
        ERR_THROW_IF(HasAction(key), "Action is not found.");
        ERR_THROW_IF_NOT(action, "Action is empty.");
        m_actions[key] = std::move(action);
      }

      void Set(const Key& key, Action action)
      {
        ERR_THROW_IF_NOT(action, "Action is empty.");
        m_actions[key] = std::move(action);
      }

      void Remove(const Key& key)
      {
        ERR_THROW_IF(HasAction(key), "Action is not found.");
        m_actions.erase(m_actions.find(key));
        return true;
      }

      bool Execute(const Key& key) const
      {
        if (const Action* action = Find(key))
        {
          (*action)();
          return true;
        }
        return false;
      }

    private:
      Map m_actions;
    };
  }
}

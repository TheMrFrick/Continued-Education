import { useState, useEffect } from 'react';
import type { Task } from './types';
import { weightedPick, generateId, loadTasks, saveTasks } from './utils';
import './App.css';

const PRIORITY_LABELS: Record<number, string> = {
  1: 'Low',
  2: 'Med-Low',
  3: 'Medium',
  4: 'Med-High',
  5: 'High',
};

export default function App() {
  const [tasks, setTasks] = useState<Task[]>(() => loadTasks());
  const [name, setName] = useState('');
  const [priority, setPriority] = useState<1 | 2 | 3 | 4 | 5>(3);
  const [picked, setPicked] = useState<Task | null>(null);
  const [shaking, setShaking] = useState(false);

  useEffect(() => {
    saveTasks(tasks);
  }, [tasks]);

  function addTask() {
    const trimmed = name.trim();
    if (!trimmed) return;
    setTasks((prev) => [...prev, { id: generateId(), name: trimmed, priority }]);
    setName('');
    setPriority(3);
  }

  function removeTask(id: string) {
    setTasks((prev) => prev.filter((t) => t.id !== id));
    if (picked?.id === id) setPicked(null);
  }

  function pick() {
    const result = weightedPick(tasks);
    setPicked(result);
    setShaking(true);
    setTimeout(() => setShaking(false), 500);
  }

  return (
    <div className="app">
      <header className="app-header">
        <h1>What Next?</h1>
        <p>Add tasks, set priorities, let it pick one for you.</p>
      </header>

      <section className="add-section">
        <input
          type="text"
          placeholder="What could you do?"
          value={name}
          onChange={(e) => setName(e.target.value)}
          onKeyDown={(e) => e.key === 'Enter' && addTask()}
          className="task-input"
        />
        <div className="priority-row">
          <span className="priority-label-text">Priority:</span>
          <div className="priority-buttons">
            {([1, 2, 3, 4, 5] as const).map((p) => (
              <button
                key={p}
                className={`priority-btn${priority === p ? ' active' : ''} p${p}`}
                onClick={() => setPriority(p)}
                title={PRIORITY_LABELS[p]}
              >
                {p}
              </button>
            ))}
          </div>
          <span className="priority-name">{PRIORITY_LABELS[priority]}</span>
        </div>
        <button className="add-btn" onClick={addTask} disabled={!name.trim()}>
          Add
        </button>
      </section>

      {tasks.length > 0 && (
        <section className="task-list">
          <h2>Tasks <span className="count">({tasks.length})</span></h2>
          <ul>
            {tasks.map((task) => (
              <li
                key={task.id}
                className={`task-item${picked?.id === task.id ? ' picked' : ''}`}
              >
                <span className={`dot p${task.priority}`} title={`Priority ${task.priority}`} />
                <span className="task-name">{task.name}</span>
                <span className={`tag p${task.priority}`}>P{task.priority}</span>
                <button className="remove-btn" onClick={() => removeTask(task.id)}>×</button>
              </li>
            ))}
          </ul>
        </section>
      )}

      <section className="pick-section">
        <button
          className={`pick-btn${shaking ? ' shake' : ''}`}
          onClick={pick}
          disabled={tasks.length === 0}
        >
          Pick for me
        </button>

        {picked && (
          <div className="result">
            <span className="result-label">Do this next:</span>
            <span className="result-name">{picked.name}</span>
            <span className={`tag p${picked.priority}`}>P{picked.priority} — {PRIORITY_LABELS[picked.priority]}</span>
          </div>
        )}

        {tasks.length === 0 && (
          <p className="empty-hint">Add some tasks above to get started.</p>
        )}
      </section>
    </div>
  );
}

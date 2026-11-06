import type { Task } from './types';

export function weightedPick(tasks: Task[]): Task | null {
  if (tasks.length === 0) return null;
  const totalWeight = tasks.reduce((sum, t) => sum + t.priority, 0);
  let rand = Math.random() * totalWeight;
  for (const task of tasks) {
    rand -= task.priority;
    if (rand <= 0) return task;
  }
  return tasks[tasks.length - 1];
}

export function generateId(): string {
  return Math.random().toString(36).slice(2, 9);
}

const STORAGE_KEY = 'what-next-tasks';

export function loadTasks(): Task[] {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    return raw ? (JSON.parse(raw) as Task[]) : [];
  } catch {
    return [];
  }
}

export function saveTasks(tasks: Task[]): void {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(tasks));
}

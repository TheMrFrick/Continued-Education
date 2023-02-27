import React from 'react'
import { Todo } from '../shared/model';
import { AiFillEdit, AiFillDelete } from 'react-icons/ai';
import { MdDone } from 'react-icons/md';

interface Props {
    todo: Todo,
    key: number,
    todos: Todo[],
    setTodos:  React.Dispatch<React.SetStateAction<Todo[]>>;
}

const SingleTodo: React.FC<Props> = ({todo, key, todos, setTodos}) => {
  return (
    <form className="todosSingle" action="">
        <span className="todosSingleText">{todo.todo}</span>
        <div>
            <span className="icon"><AiFillEdit /></span>
            <span className="icon"><AiFillDelete /></span>
            <span className="icon"><MdDone /></span>
        </div>
    </form>
  )
}

export default SingleTodo
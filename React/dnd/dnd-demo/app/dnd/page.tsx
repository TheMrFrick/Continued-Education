"use client";
import React, { useState } from "react";
import { DndContext, DragEndEvent, closestCenter } from "@dnd-kit/core";
import { arrayMove, SortableContext, verticalListSortingStrategy } from '@dnd-kit/sortable'
import { Draggable } from "@/components/Draggable";
import { Droppable } from "@/components/Droppable";
import { SortableItem } from "@/components/SortableItem";

export default function App() {
  const [languages, setLanguages] = useState(['Java', 'Python','Javascript' , 'Typescript']);
  const [isDropped, setIsDropped] = useState(false);
  const draggableMarkup = <Draggable>Drag me</Draggable>;

  function handleDragEnd(event: DragEndEvent) {
    // if (event.over && event.over.id === "droppable") {
    //   setIsDropped(true);
    // }
    console.log(event);
    const {active , over } = event;
    if(over && active.id !== over.id ){
      setLanguages((items) => {
        const activeIndex = items.indexOf(active.id.toString());
        const overIndex = items.indexOf(over?.id.toString());
        return arrayMove(languages, activeIndex, overIndex);
      })
    }
  }

  return (
    <DndContext onDragEnd={handleDragEnd} collisionDetection={closestCenter}>
      <div className='container p-3 text-center w-[50%] m-auto'>
        <h3>The Best programming language</h3>
        <SortableContext items={languages} strategy={verticalListSortingStrategy} >
          {/* We need components that use the useSortable hook */}
          {languages.map((lan) => (
            <>
              <SortableItem key={lan} id={lan}/>
            </>
          ))}
        </SortableContext>
      </div>
      {!isDropped ? draggableMarkup : null}
      <Droppable>{isDropped ? draggableMarkup : "Drop here"}</Droppable>
    </DndContext>
  );
}

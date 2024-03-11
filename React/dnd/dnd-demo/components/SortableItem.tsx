import { useSortable } from "@dnd-kit/sortable";
import {CSS} from '@dnd-kit/utilities';
import { Card, CardHeader, CardTitle } from "./ui/card";

export function SortableItem(props : any){
    // props.id
    
    const { attributes, listeners, setNodeRef, transform, transition } = useSortable({id: props.id});

    const style = {
        transform: CSS.Transform.toString(transform),
        transition
    }

    return (
        <div ref={setNodeRef} style={style} {...attributes} {...listeners}>
            <Card>
                <CardHeader>
                    <CardTitle>{props.id}</CardTitle>
                </CardHeader>
            </Card>
        </div>
    )
}
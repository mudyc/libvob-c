

#ifndef LOB_COMPONENTS_ANIMATION_H
#define LOB_COMPONENTS_ANIMATION_H

/* Animation. The beast of all. Difficult to make cleverly, impossible
 * to make perfectly?
 *
 * In GZigZag the animation was implemented to support changing
 * between views. This was understandable because of the views were
 * more or less cell based. The cell meaning one information item.
 *
 * In QLM there's state based animation. Animations are bound to
 * state, atrribute (e.g. x, y, scale), or timer, or such. There's
 * something very nice in this approach.
 *
 * In libvob the animation has been done traditionally between scenes.
 * If there's animation for each frame, it's harder to do scene based
 * animation. For instance animated gif may need frequent animation
 * and there isn't different scene all the time. Well, this is not a
 * big problem actually but in QML it's possible to put like two balls
 * jump in different intervals. What they have decided is more like:
 * when this changes, there's need for next scene.
 *
 * In libvob the matcher made the animation between coordinate
 * systems. But Layoutable objects are higher abstraction than
 * scene. I would like to think that QML like animations can be
 * implemented on top of layoutable object system. Will it be a
 * success, I can't say. The problem between ideas is that lobs come
 * and go, there isn't long standing object while in QML objects are
 * existing like rocks in forest and they are easy to make follow
 * things.
 *
 * What ever is the case, the interstate, state between animations,
 * may be problematic. How are the input events handled, is there
 * model for such?
 *
 * Actually, the (data) model is different than the rendered visual
 * content. Thus, it is easy to implement QML like animation by
 * attaching animation objects to data. Then we could create the lob
 * from data model item. It could be even better than what QML
 * provides because of binding to visual object.
 */

/* The design. By following my nose I will implement this by need. 
 */



#endif

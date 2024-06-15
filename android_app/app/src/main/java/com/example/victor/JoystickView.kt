package com.example.victor

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import kotlin.math.min
import kotlin.math.sqrt

class JoystickView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    private val outerCirclePaint = Paint().apply {
        color = Color.GRAY
        style = Paint.Style.FILL
    }

    private val innerCirclePaint = Paint().apply {
        color = Color.BLUE
        style = Paint.Style.FILL
    }

    private var centerX = 0f
    private var centerY = 0f
    private var baseRadius = 0f
    private var hatRadius = 0f

    private var posX = 0f
    private var posY = 0f

    private var listener: JoystickListener? = null

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        centerX = (w / 2).toFloat()
        centerY = (h / 2).toFloat()
        baseRadius = min(w, h) / 3f
        hatRadius = min(w, h) / 6f
        posX = centerX
        posY = centerY
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawCircle(centerX, centerY, baseRadius, outerCirclePaint)
        canvas.drawCircle(posX, posY, hatRadius, innerCirclePaint)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> {
                val dx = event.x - centerX
                val dy = event.y - centerY
                val distance = sqrt(dx * dx + dy * dy)
                if (distance < baseRadius) {
                    posX = event.x
                    posY = event.y
                } else {
                    posX = centerX + dx * baseRadius / distance
                    posY = centerY + dy * baseRadius / distance
                }
                listener?.onJoystickMoved((posX - centerX) / baseRadius, -(posY - centerY) / baseRadius)
                invalidate()
            }
            MotionEvent.ACTION_UP -> {
                posX = centerX
                posY = centerY
                listener?.onJoystickMoved(0f, 0f)
                invalidate()
            }
        }
        return true
    }

    fun setJoystickListener(listener: JoystickListener) {
        this.listener = listener
    }

    interface JoystickListener {
        fun onJoystickMoved(x: Float, y: Float)
    }
}
